#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

#include "pagezonemodel.h"
#include "pagemodel.h"
#include "pagezoneview.h"
#include "settingsmanager.h"
#include "pageview.h"

PageZoneView::PageZoneView(PageZoneModel *zone)
{
    //
    mZone = zone;
    mLastScale = -1;

    //
    mBorderWidth = settingsManager->rectItemEdgeWidth();
    mMinEdgeSize = settingsManager->zoneMinEdgeSize();

    //
    setupZoneDepent();

    //
    //connect( mZone, SIGNAL( zoneTypeChanged() ), this, SLOT( onZoneTypeChanged()) );
}

void PageZoneView::setTitle(const QString &title)
{
    //
    mTitle = title;

    //update static text
    mStaticText = QStaticText(mTitle);
    mStaticText.prepare();

    //schedule redraw
    update();
}

QRectF PageZoneView::boundingRect() const
{
    return QRectF(mZone->getPolygon().boundingRect());
}

QPainterPath PageZoneView::shape() const
{
    QPainterPath path;
    path.addPolygon(mZone->getPolygon());
    return path;
}

void PageZoneView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);

    //
    qreal scale = 1;
    PageView *view = dynamic_cast<PageView*>(widget->parentWidget());
    if (view)
        scale = view->getZoom();
    setupZoneDepent(scale);

    //rect
    qreal dd = pen().widthF() * 0.5f;
    QRectF bounds = boundingRect();
    bounds = QRectF(bounds.x() + dd, bounds.y() + dd, bounds.width() - dd - dd, bounds.height() - dd - dd);

    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(bounds);

    //title
    QTransform initialTransform = painter->worldTransform();
    qreal _scale = 1 / scale;
    painter->setWorldTransform(QTransform().scale(_scale, _scale), true);
    // TODO
    /*
    painter->drawStaticText(scale * mZone->x(), scale * mZone->y(), mStaticText );
    painter->setWorldTransform(initialTransform, false);
    */
}

void PageZoneView::setLeft(qreal x)
{
    if (x < 0) x = 0;

    //
    prepareGeometryChange();

    // TODO

    /*
    //
    int right = mZone->right();
    mZone->setLeft(x);

    if (mZone->width() < mMinEdgeSize)
    {
        mZone->setRight(right);
        mZone->setLeft(right - mMinEdgeSize);
    }
    */
}

void PageZoneView::setTop(qreal y)
{
    if (y < 0) y = 0;

    //
    prepareGeometryChange();

    // TODO

    /*
    //
    int top = mZone->top();
    mZone->setTop(y);

    if (mZone->height() < mMinEdgeSize)
    {
        mZone->setBottom(top + mMinEdgeSize);
        mZone->setTop(top);
    }
     */
}

void PageZoneView::setRight(qreal x)
{
    qreal w = scene()->sceneRect().width() - 1;
    if (x > w) x = w;

    //
    prepareGeometryChange();

    // TODO

    /*
    //
    int left = mZone->left();
    mZone->setRight(x);

    if (mZone->width() < mMinEdgeSize)
    {
        mZone->setLeft(left);
        mZone->setRight(left + mMinEdgeSize);
    }
     */
}

void PageZoneView::setBottom(qreal y)
{
    qreal h = scene()->sceneRect().height() - 1;
    if (y > h) y = h;

    //
    prepareGeometryChange();

    // TODO

    /*
    //
    int bottom = mZone->bottom();
    mZone->setBottom(y);

    if (mZone->height() < mMinEdgeSize)
    {
        mZone->setTop(bottom - mMinEdgeSize);
        mZone->setBottom(bottom);
    }
     */
}

void PageZoneView::translate(qreal dx, qreal dy)
{
    prepareGeometryChange();
    mZone->getPolygon().translate(int(dx), int(dy));

    // TODO

    /*
    //oops, we are away
    if (!scene()->sceneRect().contains(*mZone))
    {
        //
        qreal w = scene()->sceneRect().width() - 1;
        qreal h = scene()->sceneRect().height() - 1;

        int l = mZone->left();
        int r = mZone->right();
        int t = mZone->top();
        int b = mZone->bottom();

        if (dx < 0 && l < 0)
            mZone->translate(-l, 0);

        if (dx > 0 && r > w)
            mZone->translate(w - r, 0);

        if (dy > 0 && b > h)
            mZone->translate(0, h - b);

        if (dy < 0 && t < 0)
            mZone->translate(0, -t);
    }
    */
}

void PageZoneView::setupZoneDepent(qreal scale)
{
    if (mLastScale != scale)
    {
        //
        mLastScale = scale;

        //color
        QColor zoneColor = PageZoneModel::zoneColor(mZone->zoneType());

        //pen
        QPen pen = QPen(zoneColor);
        pen.setWidth( mLastScale > 0 ? mBorderWidth / mLastScale : mBorderWidth);
        setPen(pen);

        //brush
        zoneColor.setAlphaF(0.2);
        QBrush brush(zoneColor);
        setBrush(brush);
    }
}

void PageZoneView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (mZone->zoneType() != ZT_Sheet)
    {
        event->ignore();
        return;
    }

    event->accept();
    qDebug() << "Mouse move on sheet";
}
