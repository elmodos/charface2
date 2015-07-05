#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "pagelistview.h"
#include "cfplugininterface.h"
#include "zone.h"

class DocumentModel;
class PageGraphicsScene;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, ScrollItemViewDelegate
{
    Q_OBJECT
    
public:
    explicit MainWindow(DocumentModel &document, QWidget *parent = 0);
    ~MainWindow();

    void show();

    //ScrollItemViewDelegate methods
    int plvItemsCount();
    void plvSetupItem(int index, PageItemWidgetRef itemWidget);

    PageGraphicsScene *pageView() { return mPageView; }

private slots:
    void onPlugins();
    void onLoadDir();
    void onLoadFile();
    bool onBatchNew();
    bool onBatchOpen();
    bool onBatchSaveAs();
    void onEditImage(bool down);

    void updateUI();
    void updatePluginDepent();
    void updatePagesListWidget();

    void onPluginAction();
    void onPluginEditImageChanged(int index);

    void onPageListSelectionChanged(int index);
    void onPageViewChangedPage();

    void onZoom();
    void onSelectTypeChanged();
    void onMouseModeChanged();

    void onPageListDelete();
    void onPageListMove();

    void onRead();
    void onReadAll();

    void onAnalyze();
    void onAnalyzeAll();

private:
    void updateToolbar();
    void updateImageEditPluginsStackWidget();
    void updateSelectType(ZoneType zoneType);
    void updateStatusBar();

    //
    void addBatchActions();
    void addImportActions();
    void addEditImageActions();
    void addAnalyzeActions();
    void addReadActions();
    void addExportActions();
    void addPuginsActions();
    void setupStatusBar();

    void setupDropDownPluginsMenu(PluginType pluginType, QMenu *menu);

    //
    void execPluginActionImport(CFPluginInterface *plugin);

    //
    bool askToSaveBatchIsOk();

    //
    void showPluginsDialog();

    //
    Ui::MainWindow *ui;

    //
    DocumentModel *mDocument;
    int mCurrentPageIndex;

    //
    void setPageScale(qreal scale);
    PageGraphicsScene *mPageView;

    //
    PageListView *mItemsListView;

    //
    QLabel *mLabelBatchInfo;
    QLabel *mLabelPageInfo;

    //toobar buttons
    QAction *defaultReadAction;
    QMenu *menuAnalyze;

    QAction *defaultAnalyzeAction;
    QMenu *menuRead;
};

#endif // MAINWINDOW_H
