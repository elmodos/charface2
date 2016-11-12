#ifndef PAGELISTVIEW_H
#define PAGELISTVIEW_H

#include <QWidget>
#include <QScrollArea>
#include <QListView>
#include "typedefs.h"

class DocumentListView;
class PageItemWidget;
class DocumentModel;

typedef PageItemWidget* PageItemWidgetRef;
typedef QVector<PageItemWidgetRef> WidgetList;

class DocumentListView : public QListView
{
    Q_OBJECT
    
public:
    explicit DocumentListView(QWidget *parent = 0);
    ~DocumentListView();

    void setDocument(DocumentModel *document);
    void reload();
    void clearSelection(bool refresh);

    IntList selection() { return mFocusIndexes; }
    void setSelection(const IntList &list);

signals:
    void itemFocused(int);
    void itemsMultiselectionChanged();

private slots:
    void onVerticalScroll(int);

protected:
    void resizeEvent(QResizeEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    //
    void updateWidgets(bool reconfigureVisibles = false);
    void setupPageItem(const int index, PageItemWidgetRef itemWidget);

    //
    void createOverlayWidget();
    QWidget *mOverlayWidget;

private:
    DocumentModel *mDocument;
    BoolList mFocuses;
    IntList mFocusIndexes;
    WidgetList mWidgets;
    WidgetList mWidgetsReusable;
    int mItemsCount;
    int mItemHeight;
};

#endif // PAGELISTVIEW_H
