#ifndef PAGELISTVIEW_H
#define PAGELISTVIEW_H

#include <QWidget>
#include <QScrollArea>

#include "pageitemwidget.h"
#include "typedefs.h"

class PageListView;

typedef PageItemWidget* PageItemWidgetRef;
typedef QVector<PageItemWidgetRef> WidgetList;

class ScrollItemViewDelegate
{
public:
    virtual int plvItemsCount() = 0;
    virtual void plvSetupItem(int index, PageItemWidgetRef itemWidget) = 0;
};

class PageListView : public QScrollArea
{
    Q_OBJECT
    
public:
    explicit PageListView(QWidget *parent = 0);
    ~PageListView();

    void setDelegate(ScrollItemViewDelegate *delegate) { mDelegate = delegate; }
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

    //
    void createOverlayWidget();
    QWidget *mOverlayWidget;

private:
    ScrollItemViewDelegate *mDelegate;
    BoolList mFocuses;
    IntList mFocusIndexes;
    WidgetList mWidgets;
    WidgetList mWidgetsReusable;
    int mItemsCount;
    int mItemHeight;
};

#endif // PAGELISTVIEW_H
