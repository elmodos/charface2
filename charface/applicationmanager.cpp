#include <QApplication>
#include <QDir>
#include <QtConcurrent/QtConcurrentRun>
#include <unistd.h>

#include "applicationmanager.h"
#include "progressdialog.h"
#include "pluginmanager.h"
#include "settingsmanager.h"
#include "batch.h"

ApplicationManager * ApplicationManager::mInstance = NULL;

ApplicationManager::ApplicationManager()
{
    mBatch = NULL;
    mProgressDialog = NULL;

    batchNew();
}

ApplicationManager::~ApplicationManager()
{
    delete mBatch;
}

void ApplicationManager::createInstance()
{
    if (!mInstance)
        mInstance = new ApplicationManager();
}

void ApplicationManager::destroyInstance()
{
    if (mInstance)
    {
        delete mInstance;
        mInstance = NULL;
    }
}

ApplicationManager *ApplicationManager::instance()
{
    if (mInstance == NULL)
        createInstance();
    return mInstance;
}

bool ApplicationManager::batchNew(const QString &path)
{
    //
    if (mBatch)
        delete mBatch;

    //
    mBatch = new Batch(path);

    //
    emit batchModified();

    //
    return true;
}

bool ApplicationManager::batchOpen(const QString &path)
{
    //
    batchNew(path);
    mBatch->loadFromDir(path);

    //
    emit batchModified();

    //
    return true;
}

bool ApplicationManager::batchSaveAs(const QString &newPath)
{
    //
    mBatch->saveToDir(newPath);
    batchOpen(newPath);

    //
    return true;
}

void ApplicationManager::addFilesToBatch(const QStringList &list, const QString &tempDirPath, bool interactive)
{
    if (!list.size())return;

    if(interactive)
    {
        mProgressDialog = new ProgressDialog();
        mProgressDialog->setMax(list.count());
        mProgressDialog->setVal(0);
        mProgressDialog->setWindowTitle(tr("Please wait..."));
        mProgressDialog->setProgressFormat("Processed %v of %m");
        mProgressDialog->setText(tr("Adding files to batch"));
        mProgressDialog->setCanCloseWindowForMessageTypes(ProgressDialog::MI_Info);
        mProgressDialog->show();
    }

    //
    int index = 0;
    foreach(QString fileName, list)
    {
        bool isInTemp = (tempDirPath.length() && fileName.indexOf(tempDirPath) == 0);
        bool added = ApplicationManager::instance()->batch()->addFile(fileName, isInTemp);

        QString fn = QFileInfo(fileName).fileName();

        if (mProgressDialog)
        {
            mProgressDialog->setVal(++index);
            if(added)
                mProgressDialog->addMessage(tr("Added file %1").arg(fn), ProgressDialog::MI_Info);
            else
                mProgressDialog->addMessage(tr("Skipping %1").arg(fn), ProgressDialog::MI_Error);
        }
    }

    if(mProgressDialog)
    {
        mProgressDialog->setWindowTitle(tr("Done"));
        mProgressDialog->closeIfNoMessages();
    }
}

void ApplicationManager::readPages(const IntList &indexes)
{
    Q_UNUSED(indexes);
    qDebug() << "void ApplicationManager::readPage(int index)";

    // Page
    Page *page = 0; // mBatch->pages()->at( indexes );

    // WARN: Problem is here! Pointer page == 0!

    int zonesCount = page->zones()->count();

    //create and setup dialog
    ProgressDialog *dialog = new ProgressDialog();
    dialog->setMax( page->zones()->size() );
    dialog->setVal(0);
    dialog->setWindowTitle(tr("Please wait..."));
    dialog->setProgressFormat("Processed %v of %m");
    dialog->setText(tr("Recognizing..."));
    dialog->setCanCloseWindowForMessageTypes(ProgressDialog::MI_Info);
    dialog->show();
    QApplication::instance()->processEvents();

    QImage *img = new QImage( page->fileNameAbsolute() );
    CFPluginOCRInterface *plugin = pluginManager->defaultOCRPlugin();

    if (zonesCount)
    {
        //zones
        for (int i = 0; i < zonesCount; i++)
        {
            QString result;
            Zone *zone = page->zones()->at(i);
            QImage fragment = img->copy( zone->normalized() );
            bool isOk = plugin->doOCR(fragment, result);
            qDebug() << isOk << result;
        }
    }
    else
    {
        //whole page
        QString result;
        bool isOk = plugin->doOCR(*img, result);
        qDebug() << isOk << result;
    }

    dialog->closeIfNoMessages();
    delete img;
}


void ApplicationManager::analyzePages(const IntList &indexes)
{
    qDebug() << "void ApplicationManager::analyzePages(const IntList &indexes)";

    //skip empty list
    if (!indexes.count())
        return;

    //delete last dialog
    if (mProgressDialog)
    {
        mProgressDialog->close();
        mProgressDialog = NULL;
    }

    //display progress
    mProgressDialog = new ProgressDialog();
    mProgressDialog->setMax( indexes.size() );
    mProgressDialog->setVal(0);
    mProgressDialog->setWindowTitle(tr("Please wait..."));
    mProgressDialog->setProgressFormat("Processed %v of %m");
    mProgressDialog->setText(tr("Analyzing..."));
    mProgressDialog->setCancelButtonVisible(true);
    mProgressDialog->setCanCloseWindowForMessageTypes(ProgressDialog::MI_Info);

    //handle dialog's cancel
    connect(mProgressDialog, SIGNAL( cancelled() ), this, SLOT( onCancelAnalyze() ));

    //subsribe for progress updates
    connect(this, SIGNAL( progressChanged(int) ), this, SLOT( onProgressChanged(int) ));
    connect(this, SIGNAL( enableMainWindow(bool) ), this, SLOT( onEnableMainWindow(bool) ));
    connect(this, SIGNAL( finishedAnalyze() ), this, SLOT( onFinishedAnalyze() ) );

    //
    mCancelFlag = false;

    //run background process
    QtConcurrent::run(this, &ApplicationManager::analyzePagesBackground, indexes);

    //
    mProgressDialog->exec();
}

void ApplicationManager::analyzePagesBackground(const IntList &indexes)
{
    //get plugin for this job
    CFPluginAnalyzeInterface *plugin = pluginManager->defaultAnalyzePlugin();

    //run
    int count = indexes.size();
    for (int i = 0; i < count; i++)
    {
        //
        int index = indexes[i];

        //get page
        Page *page = mBatch->pages()->at(index);

        //load image file
        QImage *img = new QImage( page->fileNameAbsolute());
        ZoneList list;
        bool isOk = plugin->doAnalyze(*img, list);
        delete img;

        //
        if (isOk)
            page->mergeZones( list );

        //
        emit progressChanged(i + 1);

        //
        if (mCancelFlag)
            break;

        //test
        usleep(100000);
    }

    //rewrite batch xml file
    mBatch->saveXML();

    //
    emit finishedAnalyze();
}

void ApplicationManager::addFilesToBatchBackground(const QStringList &list,
                                                   const QString &tempDirPath,
                                                   bool interactive)
{
    Q_UNUSED(list);
    Q_UNUSED(tempDirPath);
    Q_UNUSED(interactive);
}


void ApplicationManager::onProgressChanged(int value)
{
    //update dialog value from background thread
    if (mProgressDialog)
    {
        mProgressDialog->setVal(value);
    }
}

void ApplicationManager::onCancelAnalyze()
{
    qDebug() << "ApplicationManager::onCancelAnalyze()";
    mCancelFlag = true;
}

void ApplicationManager::onFinishedAnalyze()
{
    qDebug() << "void ApplicationManager::onFinishedAnalyze();";

    //unsubsribe from progress updates
    disconnect(this, SIGNAL( progressChanged(int) ), this, SLOT( onProgressChanged(int) ));
    disconnect(this, SIGNAL( enableMainWindow(bool) ), this, SLOT( onEnableMainWindow(bool) ));
    disconnect(this, SIGNAL( finishedAnalyze() ), this, SLOT( onFinishedAnalyze() ) );

    //
    if (mProgressDialog)
    {
        mProgressDialog->closeIfNoMessages();
        mProgressDialog = NULL;
    }
}
