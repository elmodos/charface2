#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <algorithm>
#include <QScrollBar>

#include "utilities.h"
#include "pluginmanager.h"
#include "uimanager.h"
#include "settingsmanager.h"
#include "pagegraphicsscene.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "applicationmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //
    ui->setupUi(this);
    setWindowTitle(SettingsManager::instance()->mainWindowTitle());
    ui->menubar->hide();

    //
    mCurrentPageIndex = -1;

    //add items view
    mItemsListView = new PageListView();
    mItemsListView->setDelegate(this);
    ui->widget->layout()->addWidget(mItemsListView);
    connect( mItemsListView, SIGNAL( itemFocused(int) ), this, SLOT( onPageListSelectionChanged(int) ) );

    //set icons
    ui->buttonPageTop->setIcon(QIcon::fromTheme("go-top"));
    ui->buttonPageUp->setIcon(QIcon::fromTheme("go-up"));
    ui->buttonPageDown->setIcon(QIcon::fromTheme("go-down"));
    ui->buttonPageBottom->setIcon(QIcon::fromTheme("go-bottom"));
    ui->buttonPageDelete->setIcon(QIcon::fromTheme("edit-delete"));

    //
    ui->buttonZoomIn->setIcon(QIcon::fromTheme("zoom-in"));
    ui->buttonZoomOut->setIcon(QIcon::fromTheme("zoom-out"));
    ui->buttonZoomOriginal->setIcon(QIcon::fromTheme("zoom-original"));
    ui->buttonZoomFit->setIcon(QIcon::fromTheme("gtk-zoom-fit"));
    ui->buttonZoomFitWidth->setIcon(LOAD_ICON("zoom-fit-width"));

    //
    ui->buttonSelectText->setIcon(QIcon::fromTheme("text-x-generic"));
    ui->buttonSelectImage->setIcon(QIcon::fromTheme("image-x-generic"));
    ui->buttonSelectSheet->setIcon(QIcon::fromTheme("spreadsheet"));
    ui->buttonSelectUnknown->setIcon(QIcon::fromTheme("unknown"));

    //
    ui->buttonMouseDraw->setChecked(true);
    ui->buttonMouseDraw->setIcon(QIcon::fromTheme("gtk-edit"));
    ui->buttonMouseDelete->setIcon(QIcon::fromTheme("edit-delete"));

    //setup page view
    ui->graphicsPageView->setBackgroundRole(QPalette::Window);
    ui->graphicsViewActualSize->setBackgroundRole(QPalette::Window);

    //set page scene
    mPageView = new PageGraphicsScene(this);
    mPageView->seteScalableView(ui->graphicsPageView->viewport());
    updateSelectType(mPageView->zonePenType());

    //show scene
    ui->graphicsViewActualSize->setScene(mPageView);
    ui->graphicsPageView->setScene(mPageView);
    ui->graphicsPageView->setRenderHint(QPainter::SmoothPixmapTransform, true);

    //setup handlers
    connect( mPageView, SIGNAL( pageChanged() ), this, SLOT( onPageViewChangedPage() ) );
    connect( PluginManager::instance(), SIGNAL(pluginsListChanged()), this, SLOT(updatePluginDepent()) );
    connect( appManager, SIGNAL(batchModified()), this, SLOT(updatePagesListWidget()) );
    connect( ui->toolBox, SIGNAL(currentChanged(int)), this, SLOT(onPluginEditImageChanged(int)) );

    //
    defaultReadAction = NULL;
    defaultAnalyzeAction = NULL;
    menuAnalyze = NULL;
    menuRead = NULL;

    //
    setupStatusBar();
    updateStatusBar();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPlugins()
{
    UIManager::instance()->showPluginsDialog();
}

void MainWindow::onLoadDir()
{
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::Directory);

    if (!dialog.exec()) return;

    QString path = dialog.directory().absolutePath();

    QDir dir(path);

    if (!dir.exists()) return;

    dir.setFilter(QDir::Files | QDir::Readable);

    QStringList files = dir.entryList(QDir::Files | QDir::Readable);
    qDebug() << files;

    //make file names absolute
    QStringList::Iterator itBegin = files.begin();
    int count = files.count();
    for (int i = 0; i < count; i++)
        itBegin[i] = dir.absoluteFilePath(files.at(i));

    //
    //setEnabled(false);
    appManager->addFilesToBatch(files, QString(), true);
    //setEnabled(true);

    //refresh ui
    updatePagesListWidget();
}

void MainWindow::onLoadFile()
{
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::AnyFile);

    if (!dialog.exec()) return;

    //
    QStringList files = dialog.selectedFiles();
    setEnabled(false);
    appManager->addFilesToBatch(files, QString() ,true);
    setEnabled(true);

    //refresh ui
    updatePagesListWidget();
}

bool MainWindow::askToSaveBatchIsOk()
{
    //
    bool ok = true;

    //if not saved - show dialog
    if( !appManager->batch()->isSaved() )
    {
        //
        QMessageBox messageBox(QMessageBox::Question,"Not saved","Batch is not saved. Save it now?",QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        messageBox.setDetailedText(QString("Batch is stored at temporary location \"%1\"").arg(appManager->batch()->path()));
        int answer = messageBox.exec();

        //
        switch (answer)
        {
            case QMessageBox::Yes:
                ok = onBatchSaveAs();
                break;

            case QMessageBox::No:
                break;

            case QMessageBox::Cancel:
            default:
                ok = false;
                break;
        }
    }
    return ok;
}

void MainWindow::setPageScale(qreal scale)
{
    //check range
    qreal scaleMax = settingsManager->pageScaleMax();
    qreal scaleMin = settingsManager->pageScaleMin();

    if (scale > scaleMax) scale = scaleMax;
    if (scale < scaleMin) scale = scaleMin;

    //store value
    mPageView->setScaleInView(scale);

    //scale
    ui->graphicsPageView->setZoom(scale);

    //update combo box
    int scalePercent = scale * 100;
    ui->labelZoomScale->setText(QString::number(scalePercent) + "%");
}

bool MainWindow::onBatchNew()
{
    if (askToSaveBatchIsOk())
        return appManager->batchNew();

    return false;
}

bool MainWindow::onBatchOpen()
{
    if (askToSaveBatchIsOk())
    {
        QFileDialog dialog;
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::Directory);

        if (!dialog.exec()) return false;

        return appManager->batchOpen(dialog.directory().absolutePath());
    }

    return false;
}

bool MainWindow::onBatchSaveAs()
{
    //
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::Directory);

    //
    if (!dialog.exec()) return false;

    //
    return appManager->batchSaveAs(dialog.directory().absolutePath());
}

void MainWindow::onEditImage(bool down)
{
    ui->stackedWidget->setCurrentIndex(down ? 1 : 0);
}

void MainWindow::updateUI()
{
    //updatePluginDepent();
    updatePagesListWidget();
    updateToolbar();
}

void MainWindow::updatePluginDepent()
{
    updateImageEditPluginsStackWidget();
    updateToolbar();
}

void MainWindow::onPluginAction()
{
    QAction *act = qobject_cast<QAction*>( sender() );
    CFPlugin *plugin = act->data().value<CFPlugin*>();

    //
    if (!plugin)
        return;

    qDebug() << "onPluginAction";

    //get plugin type
    PluginType type = plugin->pluginType();

    //
    switch(type)
    {
        case PT_Import:
            execPluginActionImport(plugin);
            break;

        case PT_Analyze:
        {
            QAction *action = qobject_cast<QAction*>( sender() );
            if (action)
            {
                CFPlugin *plugin = action->data().value<CFPlugin*>();
                CFPluginAnalyze *pluginAnalyze = dynamic_cast<CFPluginAnalyze*>(plugin);
                pluginManager->setDefaultAnalyzePlugin(pluginAnalyze);
                updateToolbar();
            }
            break;
        }

        case PT_OCR:
        {
            QAction *action = qobject_cast<QAction*>( sender() );
            if (action)
            {
                CFPlugin *plugin = action->data().value<CFPlugin*>();
                CFPluginOCR *pluginOCR = dynamic_cast<CFPluginOCR*>(plugin);
                pluginManager->setDefaultOCRPlugin(pluginOCR);
                updateToolbar();
            }
            break;
        }

        default:
            qDebug() << "WARN: Some plugin action arrived, ignoring";
            break;
    }
}

void MainWindow::onPluginEditImageChanged(int index)
{
    //
    PluginsList plugins = pluginManager->plugins(PT_ImageEdit);

    //
    bool ok = false;

    if (index >= 0 && index < plugins.size() )
    {
        CFPlugin *pl = plugins.at(index);

        CFPluginImageEdit *plugin = dynamic_cast<CFPluginImageEdit*>(pl);
        if(plugin)
        {
            //
            QWidget *widget = plugin->createWidgetInteraction();

            if (widget)
            {
                //
                ok = true;

                //
                QVBoxLayout *layout = new QVBoxLayout();
                layout->addWidget(widget);
                //layout->addSpacerItem(new QSpacerItem());

                //
                if (ui->widgetImageEdit->layout())
                {
                    qDeleteAll(ui->widgetImageEdit->children());
                    delete ui->widgetImageEdit->layout();
                }

                ui->widgetImageEdit->setLayout(layout);
            }
        }
    }

    if (!ok)
    {
        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(new QLabel("Error loading plugin widget"));
        //layout->addSpacerItem(new QSpacerItem());

        //
        if (ui->widgetImageEdit->layout())
        {
            qDeleteAll(ui->widgetImageEdit->children());
            delete ui->widgetImageEdit->layout();
        }

        ui->widgetImageEdit->setLayout(layout);
    }
}

void MainWindow::onPageListSelectionChanged(int index)
{
    if (!appManager->batch())
        return;

    //a hack
    mPageView->loadPage(NULL);

    if (index < 0 || index >= appManager->batch()->pages()->size())
        return;

    QLabel *label = new QLabel(tr("Loading page..."),ui->graphicsPageView);
    label->resize(ui->graphicsPageView->viewport()->size());
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->show();
    QApplication::instance()->processEvents();

    //
    mPageView->loadPage(appManager->batch()->pages()->at(index));
    delete label;

    //resize
    QGraphicsPixmapItem *item = mPageView->pageItem();
    ui->graphicsViewActualSize->setSceneRect(0,0,item->boundingRect().width(),item->boundingRect().height());
    ui->graphicsPageView->setSceneRect(0,0,item->boundingRect().width(),item->boundingRect().height());

    //scale
    ui->buttonZoomFitWidth->click();
    mCurrentPageIndex = index;

    //
    updateStatusBar();
}

void MainWindow::onPageViewChangedPage()
{
    Batch *batch = appManager->batch();
    if (batch)
        batch->saveXML();

    updateStatusBar();
}

void MainWindow::onZoom()
{
    if(sender() == ui->buttonZoomOriginal)
    {
        //store offset
        QGraphicsView *view = ui->graphicsPageView;
        QPointF topLeft = view->mapToScene(view->viewport()->rect().topLeft());

        //set scale value
        setPageScale(1);

        //restore offset
        view->ensureVisible(topLeft.x(),topLeft.y(),1,1);
    }

    if(sender() == ui->buttonZoomFit)
    {
        //
        QGraphicsView *view = ui->graphicsPageView;

        //
        QSize sz = view->maximumViewportSize();

        //
        qreal scalex = (qreal)sz.width() / view->scene()->width();
        qreal scaley = (qreal)sz.height() / view->scene()->height();
        qreal scale = std::min(scalex, scaley);
        setPageScale( scale );
    }

    if(sender() == ui->buttonZoomFitWidth)
    {
        //show scrollbar
        Qt::ScrollBarPolicy policyV = ui->graphicsPageView->verticalScrollBarPolicy();
        Qt::ScrollBarPolicy policyH = ui->graphicsPageView->horizontalScrollBarPolicy();
        ui->graphicsPageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->graphicsPageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        //
        QGraphicsView *view = ui->graphicsPageView;
        QSize sz = view->maximumViewportSize();

        //
        qreal scale = (qreal)sz.width() / view->scene()->width();
        setPageScale( scale );

        //restore scrollbar policy and scroll to top
        ui->graphicsPageView->setVerticalScrollBarPolicy(policyV);
        ui->graphicsPageView->setHorizontalScrollBarPolicy(policyH);

        ui->graphicsPageView->verticalScrollBar()->setValue(0);
    }

    if(sender() == ui->buttonZoomOut)
    {
        //
        qreal scale = ui->graphicsPageView->getZoom();
        qreal step = settingsManager->pageScaleMultiplier();

        //
        scale -= step;
        setPageScale(scale);
    }

    if(sender() == ui->buttonZoomIn)
    {
        //
        qreal scale = ui->graphicsPageView->getZoom();
        qreal step = settingsManager->pageScaleMultiplier();

        //
        scale += step;
        setPageScale(scale);
    }

}

void MainWindow::onPageListDelete()
{
    IntList selection = mItemsListView->selection();
    if (!selection.size())
        return;

    std::sort(selection.begin(), selection.end());

    qDebug() << selection;

    QMessageBox messageBox(
                QMessageBox::Question,
                tr("Delete"),
                tr("Delete %1 page(s) from batch?").arg(selection.size()),
                QMessageBox::Yes | QMessageBox::No);

    int res = messageBox.exec();

    if (res != QMessageBox::Yes)
        return;

    Batch *batch = appManager->batch();
    for (int i = selection.size() - 1; i >= 0; i--)
        batch->deletePage( selection.at(i) );

    batch->saveXML();
    updatePagesListWidget();
    onPageListSelectionChanged(-1);
}

void MainWindow::onPageListMove()
{
    IntList selection = mItemsListView->selection();
    if (!selection.size())
        return;

    std::sort(selection.begin(), selection.end());

    qDebug() << selection;

    Batch *batch = appManager->batch();
    PagesList *pages = batch->pages();

    int newPos = 0;
    IntList newSelection;

    if (sender() == ui->buttonPageTop || sender() == ui->buttonPageUp)
    {
        //
        if (sender() == ui->buttonPageTop)       newPos = 0;
        if (sender() == ui->buttonPageUp)        newPos = selection.first() - 1;
        if (newPos < 0) newPos = 0;

        //
        for (int i = 0; i < selection.size(); i++)
            pages->move(selection[i], newPos + i);

        for (int i = 0; i < selection.size(); i++)
            newSelection.append(newPos + i);
    }

    if (sender() == ui->buttonPageDown || sender() == ui->buttonPageBottom)
    {
        if (sender() == ui->buttonPageDown)      newPos = selection.last() + 1;
        if (sender() == ui->buttonPageBottom)    newPos = pages->count() - 1;
        if (newPos >= pages->count()) newPos = pages->count() - 1;

        for (int i = selection.size() - 1; i >= 0; i--)
            pages->move(selection[i], newPos - (selection.size() - 1 - i));

        for (int i = 0; i < selection.size(); i++)
            newSelection.append(newPos - i);
    }

    batch->saveXML();
    qDebug() << "new selection" << newSelection;
    mItemsListView->setSelection(newSelection);
}

void MainWindow::onRead()
{
    //set this action as default
    QAction *action = dynamic_cast<QAction*>( sender() );
    if( action )
    {
        disconnect( menuRead->menuAction(), SIGNAL( triggered() ), defaultReadAction, SLOT( trigger() ) );
        menuRead->menuAction()->setText( action->text() );
        defaultReadAction = action;
        menuRead->setDefaultAction( defaultReadAction );
        connect( menuRead->menuAction(), SIGNAL( triggered() ), defaultReadAction, SLOT( trigger() ) );
    }

    //
    settingsManager->setLastUsedReadAll(false);

    //
    //appManager->readPages();
}

void MainWindow::onReadAll()
{
    QAction *action = dynamic_cast<QAction*>( sender() );
    if( action )
    {
        disconnect( menuRead->menuAction(), SIGNAL( triggered() ), defaultReadAction, SLOT( trigger() ) );
        menuRead->menuAction()->setText( action->text() );
        defaultReadAction = action;
        menuRead->setDefaultAction( defaultReadAction );
        connect( menuRead->menuAction(), SIGNAL( triggered() ), defaultReadAction, SLOT( trigger() ) );
    }
    settingsManager->setLastUsedReadAll(true);
    //appManager->readBatch();
}

void MainWindow::onAnalyze()
{
    //update default action
    QAction *action = dynamic_cast<QAction*>( sender() );
    if( action )
    {
        disconnect( menuAnalyze->menuAction(), SIGNAL( triggered() ), defaultAnalyzeAction, SLOT( trigger() ) );
        menuAnalyze->menuAction()->setText( action->text() );
        defaultAnalyzeAction = action;
        menuAnalyze->setDefaultAction( defaultAnalyzeAction );
        connect( menuAnalyze->menuAction(), SIGNAL( triggered() ), defaultAnalyzeAction, SLOT( trigger() ) );
    }
    settingsManager->setLastUsedAnalyzeAll(false);

    //fill list
    IntList indexes = mItemsListView->selection();

    //analyze
    appManager->analyzePages(indexes);

}

void MainWindow::onAnalyzeAll()
{
    //update default action
    QAction *action = dynamic_cast<QAction*>( sender() );
    if( action )
    {
        disconnect( menuAnalyze->menuAction(), SIGNAL( triggered() ), defaultAnalyzeAction, SLOT( trigger() ) );
        menuAnalyze->menuAction()->setText( action->text() );
        defaultAnalyzeAction = action;
        menuAnalyze->setDefaultAction( defaultAnalyzeAction );
        connect( menuAnalyze->menuAction(), SIGNAL( triggered() ), defaultAnalyzeAction, SLOT( trigger() ) );
    }
    settingsManager->setLastUsedAnalyzeAll(true);

    //fill list
    IntList indexes;
    int count = appManager->batch()->pages()->count();
    for (int i = 0; i < count; i++)
        indexes.push_back(i);

    //analyze
    appManager->analyzePages(indexes);
}

void MainWindow::onSelectTypeChanged()
{
    if (sender() == ui->buttonSelectText)
        updateSelectType(ui->buttonSelectText->isChecked() ? ZT_Text : ZT_Unknown);

    if (sender() == ui->buttonSelectSheet)
        updateSelectType(ui->buttonSelectSheet->isChecked() ? ZT_Sheet : ZT_Unknown);

    if (sender() == ui->buttonSelectImage)
        updateSelectType(ui->buttonSelectImage->isChecked() ? ZT_Image : ZT_Unknown);

    if (sender() == ui->buttonSelectUnknown)
        updateSelectType(ui->buttonSelectUnknown->isChecked() ? ZT_Unknown : ZT_Text);
}

void MainWindow::onMouseModeChanged()
{
    if ( sender() == ui->buttonMouseDraw )
        ui->buttonMouseDelete->setChecked( !ui->buttonMouseDraw->isChecked() );

    if ( sender() == ui->buttonMouseDelete )
        ui->buttonMouseDraw->setChecked( !ui->buttonMouseDelete->isChecked() );

    mPageView->setDeleteWithMouseClick( sender() == ui->buttonMouseDelete );
}

void MainWindow::updatePagesListWidget()
{
    //
    mItemsListView->reload();
}

void MainWindow::updateToolbar()
{
    //reset
    ui->toolBar->clear();

    addBatchActions();

    ui->toolBar->addSeparator();

    addImportActions();
    addEditImageActions();
    addAnalyzeActions();
    addReadActions();
    addExportActions();

    //
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->toolBar->addWidget(spacer);

    addPuginsActions();
}

void MainWindow::updateImageEditPluginsStackWidget()
{
    qDebug() << "updateImageEditPluginsStackWidget";

    //
    QToolBox *toolBox = ui->toolBox;

    //clear
    while (toolBox->count())
        toolBox->removeItem(toolBox->count()-1);

    //get plugins
    PluginsList plugins = pluginManager->plugins(PT_ImageEdit);

    //
    foreach (CFPlugin *pl, plugins)
    {
        CFPluginImageEdit *plugin = dynamic_cast<CFPluginImageEdit*>(pl);
        if(plugin)
            toolBox->addItem(plugin->createWidgetTool(), plugin->icon(), plugin->name());
    }
}

void MainWindow::updateSelectType(ZoneType zoneType)
{
    //keep order of enum ZoneType
    QList<QToolButton*> buttons;
    buttons << ui->buttonSelectUnknown << ui->buttonSelectText << ui->buttonSelectSheet << ui->buttonSelectImage;

    for(ZoneType zt = ZT_Unknown; zt < ZT_Count; zt++)
    {
        if (zt >= buttons.count()) break;
        buttons.at(zt)->setChecked(zt == zoneType);
    }

    //
    mPageView->setZonePenType(zoneType);
}

void MainWindow::updateStatusBar()
{
    QString str;

    //batch
    Batch *batch = appManager->batch();
    if (batch) str = tr("%1 pages in batch").arg(batch->pages()->size());
    else str = tr("No batch loaded");
    mLabelBatchInfo->setText(str);

    //page
    Page *page = mPageView->page();
    if (page)
    {
        QString strRecognozed = (page->recognized() ? "yes" : "no");

        QSize pageSize = mPageView->pageSize();
        QString strResolution = QString("%1x%2").arg(pageSize.width()).arg(pageSize.height());

        str = tr("<b>Page:</b>%1 <b>Zones:</b>%2 <b>Recognized:</b>%3 <b>Resolution:</b>%4 <b>File:</b>%5")
                .arg(mCurrentPageIndex + 1)
                .arg(page->zones()->size())
                .arg(strRecognozed)
                .arg(strResolution)
                .arg(page->fileName());
    }
    else
        str = tr("No page loaded");
    mLabelPageInfo->setText(str);

}

void MainWindow::addBatchActions()
{
    //
    QAction *action = new QAction(LOAD_ICON("document-new"),"New", ui->toolBar);
    action->setPriority(QAction::LowPriority);
    connect( action, SIGNAL(triggered()), this, SLOT(onBatchNew()));
    ui->toolBar->addAction(action);

    //
    action = new QAction(LOAD_ICON("document-open"),"Open", ui->toolBar);
    action->setPriority(QAction::LowPriority);
    connect( action, SIGNAL(triggered()), this, SLOT(onBatchOpen()));
    ui->toolBar->addAction(action);

    //
    action = new QAction(LOAD_ICON("document-save-as"),"Save as", ui->toolBar);
    connect( action, SIGNAL(triggered()), this, SLOT(onBatchSaveAs()));
    ui->toolBar->addAction(action);

}

void MainWindow::addImportActions()
{
    //import menu
    QMenu *menu = new QMenu(ui->toolBar);
    QAction *action;

    //
    action = new QAction(QIcon::fromTheme("user-pictures"),"Add directory", menu);
    connect( action, SIGNAL(triggered()), this, SLOT(onLoadDir()));
    menu->addAction(action);

    //
    action = new QAction(LOAD_ICON("add"),"Add images", menu);
    connect( action, SIGNAL(triggered()), this, SLOT(onLoadFile()));
    menu->addAction(action);

    //
    menu->setDefaultAction(action);

    //
    menu->addSeparator();

    //add import plugins
    QAction *customAction;
    PluginsList list = PluginManager::instance()->plugins( PT_Import );
    foreach (CFPlugin* plugin, list)
    {
        customAction = new QAction(plugin->icon(), plugin->actionTitle(), menu);
        customAction->setData(QVariant::fromValue(plugin));

        //
        connect( customAction, SIGNAL(triggered()), this, SLOT(onPluginAction()));

        menu->addAction(customAction);
    }

    //setup toolbar icon using default action
    QAction *defaultAction = menu->defaultAction();
    menu->setIcon(defaultAction->icon());
    menu->setTitle(defaultAction->text());
    connect(menu->menuAction(), SIGNAL(triggered()), defaultAction, SLOT(trigger()));

    //add button with dropdown arrow on toolbar
    ui->toolBar->addAction(menu->menuAction());
}

void MainWindow::addEditImageActions()
{
    QAction *action = new QAction(LOAD_ICON("document-page-setup"),"Edit mode",ui->toolBar);
    action->setCheckable(true);
    action->setChecked(false);

    connect( action, SIGNAL(toggled(bool)), this, SLOT(onEditImage(bool)) );

    ui->toolBar->addAction(action);
}

void MainWindow::addAnalyzeActions()
{
    //Analyze menu
    menuAnalyze = new QMenu();
    setupDropDownPluginsMenu(PT_Analyze, menuAnalyze);
    ui->toolBar->addAction(menuAnalyze->menuAction());
}

void MainWindow::addReadActions()
{
    //OCR menu
    menuRead = new QMenu();
    setupDropDownPluginsMenu(PT_OCR, menuRead);
    ui->toolBar->addAction(menuRead->menuAction());
}

void MainWindow::addExportActions()
{
    //analyze menu
    QMenu *menu = new QMenu(ui->toolBar);

    //add analyze plugins
    QAction *customAction;
    PluginsList list = PluginManager::instance()->plugins( PT_Export );

    foreach (CFPlugin* plugin, list)
    {
        customAction = new QAction(plugin->icon(), plugin->actionTitle(), menu);
        customAction->setData( QVariant::fromValue(plugin) );
        //handle triggered(CFPlugin*) with onPluginAction(CFPlugin*)
        connect( customAction, SIGNAL(triggered(CFPlugin*)), this, SLOT(onPluginAction(CFPlugin*)));

        menu->addAction(customAction);
    }

    //setup toolbar icon using default action
    menu->setIcon(LOAD_ICON("document-export"));
    menu->setTitle("Save");

    //add button with dropdown arrow on toolbar
    ui->toolBar->addAction(menu->menuAction());
}

void MainWindow::addPuginsActions()
{
    QAction *action = new QAction( LOAD_ICON("document-properties"), tr("Plugins"), 0);
    connect( action, SIGNAL(triggered()), this, SLOT(onPlugins()) );
    ui->toolBar->addAction(action);
}

void MainWindow::setupStatusBar()
{
    //
    mLabelBatchInfo = new QLabel();
    mLabelBatchInfo->setFrameShape(QFrame::StyledPanel);
    ui->statusbar->addWidget(mLabelBatchInfo);

    //
    mLabelPageInfo = new QLabel();
    mLabelPageInfo->setFrameShape(QFrame::StyledPanel);
    ui->statusbar->addWidget(mLabelPageInfo);
}

void MainWindow::setupDropDownPluginsMenu(PluginType pluginType, QMenu *menu)
{
    //add analyze plugins
    QAction *customAction = NULL;
    PluginsList list = PluginManager::instance()->plugins( pluginType );

    bool useAll;
    QString textSingle;
    QString textAll;
    QString iconName;

    switch ( pluginType)
    {
        case PT_OCR:
            useAll = settingsManager->lastUsedReadAll();
            textSingle = tr("Read");
            textAll = tr("Read all");
            iconName = "edit-select-all";
            break;

        case PT_Analyze:
            useAll = settingsManager->lastUsedAnalyzeAll();
            textSingle = tr("Analyze");
            textAll = tr("Analyze all");
            iconName = "edit-find";
            break;

        default:
            break;
    }

    //
    if (list.size())
    {
        //plugin with checkmark
        CFPlugin *defaultPlugin = NULL;

        //Leading actions
        {
            QAction *action1 = menu->addAction( textAll );
            QAction *action2 = menu->addAction( textSingle );
            menu->setDefaultAction(useAll ? action1 : action2);

            switch ( pluginType)
            {
                case PT_OCR:
                    connect( action1 , SIGNAL( triggered() ), this, SLOT( onReadAll()) );
                    connect( action2 , SIGNAL( triggered() ), this, SLOT( onRead()) );
                    defaultReadAction = menu->defaultAction();
                    menu->menuAction()->setText(defaultReadAction->text());
                    connect( menu->menuAction(), SIGNAL(triggered()), defaultReadAction, SLOT(trigger()) );
                    defaultPlugin = pluginManager->defaultOCRPlugin();
                    break;

                case PT_Analyze:
                    connect( action1 , SIGNAL( triggered() ), this, SLOT( onAnalyzeAll()) );
                    connect( action2 , SIGNAL( triggered() ), this, SLOT( onAnalyze()) );
                    defaultAnalyzeAction = menu->defaultAction();
                    menu->menuAction()->setText(defaultAnalyzeAction->text());
                    connect( menu->menuAction(), SIGNAL(triggered()), defaultAnalyzeAction, SLOT(trigger()) );
                    defaultPlugin = pluginManager->defaultAnalyzePlugin();
                    break;

                default:
                    break;
            }

            menu->addSeparator();
        }

        // add plugins list
        foreach (CFPlugin* plugin, list)
        {
            customAction = new QAction(plugin->icon(), plugin->actionTitle(), menu);
            customAction->setData( QVariant::fromValue(plugin) );
            customAction->setCheckable(true);
            customAction->setChecked( defaultPlugin == plugin);

            //
            connect( customAction, SIGNAL(triggered()), this, SLOT(onPluginAction()));

            menu->addAction(customAction);
        }

        menu->addSeparator();
        QAction *actionPluginSettings = new QAction(tr("Configure %1").arg("plugin name here"), menu);
        menu->addAction(actionPluginSettings);

        //add button with dropdown arrow on toolbar
    }
    else
    {
        menu->menuAction()->setText( useAll ? textAll : textSingle );
        menu->addAction(tr("No plugins"))->setEnabled(false);
    }

    menu->menuAction()->setIcon(LOAD_ICON(iconName));

}

void MainWindow::execPluginActionImport(CFPlugin *plugin)
{
    qDebug() << "import plugin";

    //cast
    CFPluginImport *pluginImport = dynamic_cast<CFPluginImport*>(plugin);

    //use
    if(pluginImport)
    {
        //disable main window
        setEnabled(false);

        //clean temp dir
        QString tempPath = appManager->batch()->tempPath(true);
        appManager->batch()->removeAllFilesInDir(tempPath);

        //
        QStringList files = pluginImport->doImportFiles( tempPath );
        appManager->addFilesToBatch(files, tempPath, true);

        //clean temp dir and delete it
        appManager->batch()->removeAllFilesInDir(tempPath);
        QDir().rmdir(tempPath);

        //
        setEnabled(true);

        //update UI
        updatePagesListWidget();
    }
}

void MainWindow::show()
{
    updateUI();
    QMainWindow::show();
}

int MainWindow::plvItemsCount()
{
    Batch *batch = appManager->batch();

    if (batch)
        return batch->pages()->size();
    else
        return 0;
}

void MainWindow::plvSetupItem(int index, PageItemWidgetRef itemWidget)
{
    itemWidget->setTitle( QString("Page %1").arg(index + 1) );
    itemWidget->setPixmap( appManager->batch()->pages()->at(index)->thumb() );
}
