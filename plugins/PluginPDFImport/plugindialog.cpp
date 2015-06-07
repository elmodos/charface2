#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <poppler-qt4.h>

#include <numbersegment.h>

#include "plugindialog.h"
#include "ui_plugindialog.h"


PluginDialog::PluginDialog(const QString &tempDirPath) :
    QDialog(0),
    ui(new Ui::PluginDialog),
    mTempDir(tempDirPath)
{
    ui->setupUi(this);
    setWindowIcon(QIcon::fromTheme("application-pdf"));
    setWindowTitle(tr("PDF import"));

    //
    ui->labelIcon->setPixmap(QIcon::fromTheme("application-pdf").pixmap(48));
    ui->labelIconOutput->setPixmap(QIcon::fromTheme("image-tiff").pixmap(48));
    ui->buttonStart->setIcon(QIcon::fromTheme("dialog-apply"));
    ui->buttonCancel->setIcon(QIcon::fromTheme("process-stop"));
    ui->buttonBrowse->setIcon(QIcon::fromTheme("stock_folder"));

    //
    ui->buttonStart->setEnabled(false);
    ui->lineEditPages->setEnabled(false);

    //
    ui->comboBoxDPI->clear();
    ui->comboBoxDPI->addItems(QStringList() << "150" << "300" << "600");
    ui->comboBoxDPI->setCurrentIndex(1);
    ui->checkBoxMultithread->setChecked(true);

    //
    ui->labelPagesCount->setText("");

    //
    mUpdateTimer = new QTimer(this);
    connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(updateProgress()));

}

PluginDialog::~PluginDialog()
{
    delete ui;
}

void PluginDialog::onBrowse()
{
    //reset values
    mReturnList.clear();

    //select file with file dialog
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilters(QStringList() << "*.pdf" << "*");

    //
    if (!dialog.exec()) return;

    //get selected files list
    QStringList files = dialog.selectedFiles();

    //show some interaction with progress
    if (files.count())
    {
        ui->lineEdit->setText(files.at(0));
        checkFile(files.at(0));
    }
}

void PluginDialog::onStart()
{
    //
    setUiLocked(true);
    mFileNames.clear();

    //get selected pages ranges
    bool errorr = true;
    mAllPageList = NumberSegment::getIndexesList(ui->lineEditPages->text(), errorr);
    ui->progressBar->setMaximum(mAllPageList.size());

    //decrease all indexes by 1
    IntList::Iterator itBegin = mAllPageList.begin();
    int count = mAllPageList.size();
    for (int i = 0; i < count; i++)
        (itBegin[i])--;

    //split pages to sublist by cpus
    int cpuCount = (ui->checkBoxMultithread->isChecked() ? QThread::idealThreadCount() : 1);
    int segmentSize = mPagesCount / cpuCount;

    //
    stopAllThreads();

    //
    int dpi = ui->comboBoxDPI->currentText().toInt();

    //setup threads
    for(int i = 0; i < cpuCount; i++)
    {
        int segmentStart = i * segmentSize;
        IntList indexesList = mAllPageList.mid(segmentStart, (i == cpuCount - 1 ?  mAllPageList.size() - segmentStart : segmentSize));

        PDFRenderThread *thread = new PDFRenderThread(ui->lineEdit->text(), indexesList, &mProcessedPageCount, &mIncMutex, mTempDir, dpi);
        mThreadList.append( thread );
        qDebug() << "Thread " << i << "to render indexes " << indexesList;
    }

    //run and keep calm :)
    mProcessedPageCount = 0;
    for(int i=0; i< mThreadList.count(); i++)
        mThreadList.at(i)->start();

    //
    mUpdateTimer->start(500);
}

void PluginDialog::onCancel()
{
    stopAllThreads();
    reject();
}

void PluginDialog::onPagesChanged(const QString &newText)
{
    bool error = false;
    IntList list = NumberSegment::getIndexesList(newText, error);

    if(!error)
        foreach (int i, list)
            if (i < 1 || i > mPagesCount)
            {
                error = true;
                break;
            }

    ui->buttonStart->setEnabled(!error);
}

void PluginDialog::updateProgress()
{
    qDebug() << "Update ui";
    bool isRunning = false;
    for(int i = 0; i < mThreadList.count(); i++)
        isRunning |= mThreadList.at(i)->isRunning();

    qDebug() << "Still running" << (isRunning ? "true" : "false");

    mIncMutex.lock();
    int newProgress = mProcessedPageCount;
    mIncMutex.unlock();

    ui->progressBar->setValue(newProgress);

    if (!isRunning)
    {
        mUpdateTimer->stop();
        setUiLocked(false);

        foreach (PDFRenderThread *thread, mThreadList)
            mFileNames.append(thread->processedFiles());

        onCancel();
    }
}

void PluginDialog::checkFile(const QString &fileName)
{
    //try load document
    Poppler::Document *document = Poppler::Document::load(fileName);

    if (document && !document->isLocked())
    {
        mPagesCount = document->numPages();
        ui->labelPagesCount->setText(tr("%1 pages").arg(mPagesCount));
        ui->progressBar->setMaximum(mPagesCount);
        ui->lineEditPages->setText(QString::number(1)+"-"+QString::number(mPagesCount));
        ui->buttonStart->setEnabled(true);
        ui->lineEditPages->setEnabled(true);
    }
    else if(document && document->isLocked())
    {
        QMessageBox::critical(this,tr("Error"), tr("Password protected files are not supported yet."));
    }
    else
    {
        ui->progressBar->setMaximum(1);
        ui->lineEdit->setText("");
        ui->buttonStart->setEnabled(false);
        ui->lineEditPages->setEnabled(false);
        QMessageBox(QMessageBox::Critical,tr("Error"),tr("Bad PDF file"),QMessageBox::Ok).exec();
    }

    delete document;
}


void PluginDialog::setUiLocked(bool val)
{
    ui->groupBoxInput->setEnabled(!val);
    ui->groupBoxOutput->setEnabled(!val);
    ui->buttonStart->setEnabled(!val);
}

void PluginDialog::stopAllThreads()
{
    //disable window
    setEnabled(false);
    QApplication::instance()->processEvents();

    //stop threads
    foreach(PDFRenderThread *thread, mThreadList)
        thread->stop();

    //wait for finish
    while (true)
    {
        bool isRunning = false;

        for(int i = 0; i < mThreadList.count(); i++)
            isRunning |= mThreadList.at(i)->isRunning();

        if (!isRunning) break;
    }

    qDeleteAll(mThreadList);
    mThreadList.clear();

    setEnabled(true);
}

void PluginDialog::closeEvent(QCloseEvent *)
{
    stopAllThreads();
}
