#include <QDebug>
#include <QRect>
#include <QVBoxLayout>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>
#include <math.h>

#include "settingsmanager.h"
#include "UI/MainWindow/PageGraphicsScene/PageGraphicsScene.h"
#include "pagezoneview.h"
#include "Models/PageZoneModel.h"

PageGraphicsScene::PageGraphicsScene(QWidget *parent)
    : QGraphicsScene(parent),
      mPageWidth(0),
      mPageHeight(0),
      mDeleteWithMouseClick(false)
{
    //
    mHasImage = false;
    mPixmapItem = NULL;

    //
    mPage = NULL;

    //
    mCurrentItem = NULL;

    //
    mMoveState = MS_None;
    mMouseIsDown = false;
    mPressedButton = Qt::NoButton;

    //
    mZonePenType = ZT_Text;

    //
    mScalableView = NULL;
    mScaleInView = 1.0;
    edgeMagnetDistance = settingsManager->zoneRectEdgeMagnetDistance();
}

bool PageGraphicsScene::loadPage(PageModel *page)
{
    //
    mPage = page;

    //clear scene
    clear();
    setSceneRect(0, 0, 0, 0);
    mHasImage = false;
    mPixmapItem = NULL;
    mPageHeight = 0;
    mPageWidth = 0;
    mCurrentItem = NULL;
    mMoveState = MS_None;

    //
    if (!mPage)
        return false;

    //try load image
    QPixmap pixmap(mPage->fileNameAbsolute());
    if (pixmap.isNull())
        return false;

    //image size
    mPageWidth = pixmap.width();
    mPageHeight = pixmap.height();
    QRect pageRect(0, 0, mPageWidth, mPageHeight);
    setSceneRect(pageRect);

    //add page image
    mPixmapItem = addPixmap(pixmap);
    mPixmapItem->setTransformationMode(Qt::SmoothTransformation);
    mHasImage = true;

    //
    createRects();

    //
    return true;
}

void PageGraphicsScene::repaintRects()
{
    foreach( QGraphicsItem *item, items())
        if (item != mPixmapItem)
            delete item;

    createRects();
}

void PageGraphicsScene::onChangeZoneType()
{
    QAction *action = qobject_cast<QAction*>( sender() );

    if (!action) return;
    int tag = action->data().toInt();

    if (mCurrentItem)
    {
        mCurrentItem->getZone()->setZoneType(tag);
        mCurrentItem->setupZoneDepent();
        update();
        emit pageChanged();
    }
}

void PageGraphicsScene::onDeleteZone()
{
    if (!mCurrentItem)
        return;

    items();
    RectList *zones = mPage->zones();
    for (RectList::iterator it = zones->begin(); it != zones->end(); ++it)
        if (*it == mCurrentItem->getZone())
        {
            zones->erase(it);
            removeItem( mCurrentItem );
            mCurrentItem = NULL;
            break;
        }
    update();
    emit pageChanged();
}

void PageGraphicsScene::createRects()
{
    //get zones
    RectList *zones = mPage->zones();

    //
    for (int i = 0; i < zones->size(); i++)
        addItem( createRect(zones->at(i), i+1) );
}

PageZoneView *PageGraphicsScene::createRect(PageZoneModel *zone, int index)
{
    PageZoneView *rectItem = new PageZoneView(zone);
    rectItem->setTitle(QString::number(index));
    return rectItem;
}

void PageGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //inherited
    QGraphicsScene::mousePressEvent(event);

    //skip if already is down
    if (mMouseIsDown) return;

    //skip if no image loaded
    if (!mHasImage) return;

    //handle left button
    if (event->buttons() == Qt::LeftButton && !mDeleteWithMouseClick)
    {
        //
        mPressedButton = Qt::LeftButton;
        mMouseIsDown = true;

        //get item under cursor
        QPointF p = event->scenePos();
        mMovePos = p;
        // Using QTransform as temporary solution
        PageZoneView *item = dynamic_cast<PageZoneView*>( itemAt(p, QTransform()) );

        //skip pixmap item
        if (item && item->type() == QGraphicsItemCustomPolygon && mPixmapItem)
        {
            PageZoneView *rectItem = dynamic_cast<PageZoneView*>(item);
            if(rectItem)
            {
                //
                mCurrentItem = rectItem;

                //
                QRectF rect = rectItem->boundingRect();
                qreal delta = edgeMagnetDistance / (event->widget() == mScalableView ? mScaleInView : 1);

                //
                mMoveState = getMoveState(p.x(), p.y(), rect.left(), rect.top(), rect.right(), rect.bottom(), delta);
                qDebug() << "rect focused with left button";
            }
        }
        else
        //start drawing new rect
        if (mPixmapItem)
        {

            //create zone instance
            QRect rect(p.x(), p.y(), edgeMagnetDistance, edgeMagnetDistance);
            PageZoneModel *zone = new PageZoneModel(rect);
            zone->setZoneType(mZonePenType);
            mPage->zones()->append(zone);

            //create pageview rect instance
            PageZoneView *rectItem = createRect(zone, mPage->zones()->count());
            addItem(rectItem);

            // TODO
            /*
            //store state
            mMovePos = zone->bottomRight();
            mMoveState = MS_BottomRight;
            mCurrentItem = rectItem;
            qDebug() << "new rect draw started";
            */

        }
    }

    if (event->buttons() == Qt::LeftButton && mDeleteWithMouseClick)
    {
        //get item under cursor
        QPointF p = event->scenePos();
        mMovePos = p;
        // Using QTransform() as temporary solution
        PageZoneView *item = dynamic_cast<PageZoneView*>( itemAt(p, QTransform()) );

        //skip pixmap item
        if (item && item->type() == QGraphicsItemCustomPolygon && mPixmapItem)
        {
            PageZoneView *rectItem = dynamic_cast<PageZoneView*>(item);
            if(rectItem)
            {
                //find and delete zone from list
                RectList *zones = mPage->zones();
                PageZoneModel *zone = rectItem->getZone();
                for (RectList::iterator it = zones->begin(); it != zones->end(); ++it)
                    if (zone == *it)
                    {
                        zones->erase(it);
                        break;
                    }

                //delete visual representations
                removeItem(rectItem);
                update();
                emit pageChanged();
            }
        }
    }

    //handle right button
    if (event->buttons() == Qt::RightButton)
    {
        //get item under cursor
        QPointF p = event->scenePos();
        mMovePos = p;
        // Using QTransform() as temporary solution
        PageZoneView *item = dynamic_cast<PageZoneView*>( itemAt(p, QTransform()) );

        //skip pixmap item
        if (item && item->type() == QGraphicsItemCustomPolygon && mPixmapItem)
        {
            mCurrentItem = item;
            mPressedButton = Qt::RightButton;
            mMouseIsDown = true;
        }
    }

    else
        mPressedButton = Qt::NoButton;
}

void PageGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //
    QGraphicsScene::mouseReleaseEvent(event);

    //
    if(mCurrentItem && mMoveState != MS_None)
        emit pageChanged();

    //
    if (mMouseIsDown && mPressedButton == Qt::RightButton && mCurrentItem)
    {
        showContextMenu();
    }

    //
    mMoveState = MS_None;
    mCurrentItem = 0;
    mMouseIsDown = false;
}

void PageGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);

    //
    if (mMouseIsDown && !mCurrentItem)
    {
        mMouseIsDown = false;
        mPressedButton = Qt::NoButton;
    }

    //get item under cursor
    QPointF p = event->scenePos();
    // Using QTransform() as temporary solution
    QGraphicsItem* item = itemAt(p, QTransform());

    //
    bool flagCursorSet = false;

    if ( mMoveState == MS_None && item && item != mPixmapItem && mPixmapItem )
    {
        //get item frame
        QRectF itemRect = item->boundingRect().normalized();
        qreal delta = edgeMagnetDistance / (event->widget() == mScalableView ? mScaleInView : 1);

        //suppose we set cursor
        flagCursorSet = true;

        //set cursor
        if (mDeleteWithMouseClick)
        {
            flagCursorSet = true;
            mPixmapItem->setCursor(Qt::PointingHandCursor);
        }
        else
        switch ( getMoveState( p.x(), p.y(), itemRect.left(), itemRect.top(), itemRect.right(), itemRect.bottom(), delta) )
        {
            case MS_Left:
            case MS_Right:
                mPixmapItem->setCursor(Qt::SizeHorCursor);
                break;

            case MS_Top:
            case MS_Bottom:
                mPixmapItem->setCursor(Qt::SizeVerCursor);
                break;

            case MS_TopLeft:
            case MS_BottomRight:
                mPixmapItem->setCursor(Qt::SizeFDiagCursor);
                break;

            case MS_TopRight:
            case MS_BottomLeft:
                mPixmapItem->setCursor(Qt::SizeBDiagCursor);
                break;

            case MS_Move:
                mPixmapItem->setCursor(Qt::SizeAllCursor);
                break;

            case MS_None:
            default:
                flagCursorSet = false;
        }
    }
    else if (mMoveState != MS_None)
    {
        flagCursorSet = true;
        qDebug() << "move resize here";
        switch ( mMoveState )
        {
            case MS_Left:
                mCurrentItem->setLeft(p.x());
                break;

            case MS_Right:
                mCurrentItem->setRight(p.x());
                break;

            case MS_Top:
                mCurrentItem->setTop(p.y());
                break;

            case MS_Bottom:
                mCurrentItem->setBottom(p.y());
                break;

            case MS_TopLeft:
                mCurrentItem->setTop(p.y());
                mCurrentItem->setLeft(p.x());
                break;


            case MS_BottomRight:
                mCurrentItem->setBottom(p.y());
                mCurrentItem->setRight(p.x());
                break;

            case MS_TopRight:
                mCurrentItem->setTop(p.y());
                mCurrentItem->setRight(p.x());
                break;

            case MS_BottomLeft:
                mCurrentItem->setBottom(p.y());
                mCurrentItem->setLeft(p.x());
                break;

            case MS_Move:
                mCurrentItem->translate(p.x() - mMovePos.x(), p.y() - mMovePos.y());
                break;

            case MS_None:
            default:
                break;
        }
        mMovePos = p;
    }

    if (!flagCursorSet && mPixmapItem)
        mPixmapItem->setCursor(mDeleteWithMouseClick ? Qt::ForbiddenCursor : Qt::CrossCursor);
}

void PageGraphicsScene::showContextMenu()
{
    QMenu *menu = new QMenu();

    //change zone type
    QMenu *changeTypeMenu = menu->addMenu(tr("Change zone type"));
    for (ZoneType zt = 0; zt < ZT_Count; zt++)
    {
        QAction *action = changeTypeMenu->addAction( PageZoneModel::zoneToString(zt) );
        action->setData( zt );
        action->setCheckable( true );
        action->setChecked( mCurrentItem->getZone()->zoneType() == zt );
        connect( action, SIGNAL( triggered() ), this, SLOT( onChangeZoneType() ) );
    }

    //edit sheet
    if (mCurrentItem->getZone()->zoneType() == ZT_Sheet)
    {
        QMenu *changeTypeMenu = menu->addMenu(PageZoneModel::zoneToString(ZT_Sheet));

        changeTypeMenu->addAction( tr("Add vertical line") );
        changeTypeMenu->addAction( tr("Add horizontal line") );
        changeTypeMenu->addAction( tr("Delete line") );
    }

    //delete  zone
    QAction *action = menu->addAction( tr("Delete zone") );
    connect( action, SIGNAL( triggered() ), this, SLOT( onDeleteZone() ) );

    //
    menu->exec(QCursor::pos());
    delete menu;
}

MoveState PageGraphicsScene::getMoveState(qreal x, qreal y, qreal left, qreal top, qreal right, qreal bottom, qreal delta) const
{
    //shitcode :)

    //top left
    if (x <= left + delta && y <= top + delta)
        return MS_TopLeft;
    else

    //top right
    if ( x >= right - delta && y <= top + delta )
        return MS_TopRight;
    else

    //bottom left
    if ( x <= left + delta && y >= bottom - delta )
         return MS_BottomLeft;
    else

    //bottom right
    if ( x >= right - delta && y >= bottom - delta )
        return MS_BottomRight;
    else

    //left edge
    if ( x <= left + delta )
        return MS_Left;
    else

    //right edge
    if ( x >= right - delta )
        return MS_Right;
    else

    //top edge
    if ( y <= top + delta )
        return MS_Top;
    else

    //bottom edge
    if ( y >= bottom - delta)
        return MS_Bottom;
    else

    //inner area
    if ( x > left + delta && x < right - delta && y > top + delta && y < bottom - delta)
        return MS_Move;

    else
        return MS_None;
}
