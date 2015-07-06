#ifndef ZONERECTITEM_H
#define ZONERECTITEM_H

#include <QGraphicsPolygonItem>
#include <QStaticText>

class PageZoneModel;
class PageModel;

enum {
    QGraphicsItemCustomPolygon = QGraphicsItem::UserType + 1
};

class PageZoneView : public QGraphicsPolygonItem
{
public:

    //
    explicit PageZoneView(PageZoneModel *zone);

    //
    PageZoneModel *getZone() { return mZone; }

    // show text
    void setTitle(const QString &title);

    // overriden
    int type() const { return QGraphicsItemCustomPolygon; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // TODO: redo from scratch
    // move
    void setLeft(qreal x);
    void setTop(qreal y);
    void setRight(qreal x);
    void setBottom(qreal y);
    void translate(qreal dx, qreal dy);

    //
    void setupZoneDepent(qreal scale = 1);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    int mMinEdgeSize;
    PageZoneModel *mZone;
    QString mTitle;
    QStaticText mStaticText;

    //
    qreal mLastScale;
    qreal mBorderWidth;
};

#endif // ZONERECTITEM_H
