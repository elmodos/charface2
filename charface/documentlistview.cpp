#include <QScrollBar>
#include <QToolButton>
#include <QDebug>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QFlag>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QApplication>

#include "pagelistview.h"
#include "pageitemwidget.h"

PageListView::PageListView(QWidget *parent) :
    QScrollArea(parent)
{
    //
    setMouseTracking(true);
    //setBackgroundRole(QPalette::Base);
    setWidget(new QWidget());
    mItemsCount = 0;
    mDelegate = NULL;
    mItemHeight = PageItemWidget::defaultHeight();

    //
    mOverlayWidget = NULL;
    createOverlayWidget();

    //
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //
    connect( verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onVerticalScroll(int)) );
}

PageListView::~PageListView()
{
    delete mOverlayWidget;
}

void PageListView::reload()
{
    //get items count
    mItemsCount = mDelegate->plvItemsCount();

    qDeleteAll(mWidgetsReusable);
    mWidgetsReusable.clear();

    qDeleteAll(mWidgets);
    mWidgets.clear();

    //
    mFocuses.clear();
    mFocusIndexes.clear();

    //
    for (int i = 0; i < mItemsCount; i++)
    {
        mWidgets.push_back(NULL);
        mFocuses.push_back(false);
    }

    //update geometry
    widget()->resize( viewport()->width(), mItemHeight * mItemsCount );
    ensureVisible(0,0);

    //update visible items
    updateWidgets();
}

void PageListView::clearSelection(bool refresh)
{
    //clean all selection flags
    foreach (int index, mFocusIndexes)
        mFocuses[index] = false;

    //no items are selected
    mFocusIndexes.clear();

    //unselect visible widgets
    if (refresh)
        updateWidgets();
}

void PageListView::setSelection(const IntList &list)
{
    clearSelection(false);
    mFocusIndexes = list;
    foreach(int i, mFocusIndexes)
        mFocuses[i] = true;
    updateWidgets(true);
}

void PageListView::onVerticalScroll(int)
{
    updateWidgets();
}

void PageListView::resizeEvent(QResizeEvent *event)
{
    QScrollArea::resizeEvent(event);

    QWidget *w = widget();
    w->resize( viewport()->width() , w->height());
    updateWidgets();
}

void PageListView::mouseReleaseEvent(QMouseEvent *event)
{
    //get item index
    QPoint pos = widget()->mapFromParent( event->pos() );
    int index = pos.y() / mItemHeight;

    //check range
    if (index >= mItemsCount) return;

    //get item under cursor
    QWidget *w = widget()->childAt(pos);
    if (!w)return;

    //extend selection with Ctrl button
    bool isCtrlPressed = (event->modifiers() == Qt::ControlModifier);

    //skip aif button under cursor
    QAbstractButton *button = dynamic_cast<QAbstractButton*>(w);

    //not a button
    if (!button)
    {
        //1 item should stay highlightet
        bool onlyOneFocused = mFocusIndexes.size() == 1;
        bool clickedItemIsFocused = mFocuses[index];
        bool unselectingLast = onlyOneFocused && isCtrlPressed && clickedItemIsFocused;
        if (unselectingLast) return;

        if (!isCtrlPressed)
            clearSelection(true);

        //toggle value
        mFocuses[index] = !mFocuses[index];

        //set item highlight
        PageItemWidgetRef item = mWidgets[index];
        item->setHighlighted( mFocuses[index] );

        //add or remove focus index
        if (mFocuses[index])
            mFocusIndexes.push_back( index );
        else
            mFocusIndexes.remove( mFocusIndexes.lastIndexOf( index ) );

        //force item selection update before durable page load
        repaint();
        QApplication::instance()->processEvents();
        //signals
        if (!isCtrlPressed || mFocusIndexes.size() == 1)
            emit itemFocused(index);

        //
        if (isCtrlPressed && mFocusIndexes.size() > 1)
            emit itemsMultiselectionChanged();
    }
}

void PageListView::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_A)
    {
        mFocusIndexes.clear();

        for (int i = 0; i < mItemsCount; i++)
        {
            mFocuses[i] = true;
            mFocusIndexes.push_back(i);
        }

        updateWidgets();
    }
}

void PageListView::updateWidgets(bool reconfigureVisibles)
{
    int ymin = verticalScrollBar()->value();
    int ymax = ymin + viewport()->height();

    int indexmin = ymin / mItemHeight;
    int indexmax = ymax / mItemHeight + 1;
    if (indexmax >= mItemsCount) indexmax = mItemsCount - 1;

    //get reusables
    for (int i = 0; i < mItemsCount; i++)
    {
        //skip visible range
        if (i >= indexmin && i <= indexmax) continue;

        PageItemWidgetRef *iw = &mWidgets[i];
        if (*iw)
        {
            mWidgetsReusable.push_back(*iw);
            (*iw)->hide();
            *iw = NULL;
        }
    }

    //get visibles
    int itemWidth = viewport()->width();
    for (int i = indexmin; i <= indexmax; i++)
    {
        PageItemWidgetRef iw = mWidgets[i];

        //widget is on its place
        if (iw)
        {
            if (reconfigureVisibles)
                mDelegate->plvSetupItem(i,iw);
            iw->resize(itemWidth, iw->height());
            iw->setHighlighted(mFocuses[i]);
            continue;
        }

        //take from reusebles
        if (!mWidgetsReusable.isEmpty())
        {
            iw = mWidgetsReusable.last();
            mWidgetsReusable.remove(mWidgetsReusable.size() - 1);
        }
        else
        {
            iw = new PageItemWidget();
            iw->setOverlayWidget(mOverlayWidget);
        }

        mWidgets[i] = iw;
        mDelegate->plvSetupItem(i, iw);
        iw->setHighlighted(mFocuses[i]);

        QRect frame(0, i * mItemHeight, viewport()->width(), mItemHeight);
        iw->setGeometry(frame);
        iw->setParent(widget());
        iw->show();
    }
}

void PageListView::createOverlayWidget()
{
    mOverlayWidget = new QWidget(this);
    mOverlayWidget->hide();

    QVBoxLayout *layout = new QVBoxLayout(mOverlayWidget);
    layout->setSpacing(0);
    layout->setContentsMargins( QMargins(3,3,3,3) );

    QToolButton *btn = new QToolButton();
    btn->setIcon(QIcon::fromTheme("edit-delete"));
    btn->setAutoRaise(true);
    layout->addWidget(btn);

    btn = new QToolButton();
    btn->setIcon(QIcon::fromTheme("edit-clear"));
    btn->setAutoRaise(true);
    layout->addWidget(btn);

    btn = new QToolButton();
    btn->setIcon(QIcon::fromTheme("gtk-info"));
    btn->setAutoRaise(true);
    layout->addWidget(btn);

    mOverlayWidget->resize( mOverlayWidget->sizeHint());

}
