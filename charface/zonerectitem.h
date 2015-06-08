#ifndef ZONERECTITEM_H
#define ZONERECTITEM_H

#include <QGraphicsRectItem>
#include <QStaticText>

class Zone;
class Page;

enum {
    QGraphicsItemCustomRect = QGraphicsItem::UserType + 1
};

class ZoneRectItem : public QGraphicsRectItem
{
public:
    explicit ZoneRectItem(Zone *zone);

    //
    Zone *zone() { return mZone; }

    //show text
    void setTitle(const QString &title);

    //overriden
    QRectF boundingRect() const;

    QPainterPath shape() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //move
    void setLeft(qreal x);
    void setTop(qreal y);
    void setRight(qreal x);
    void setBottom(qreal y);
    void translate(qreal dx, qreal dy);

    //
    void setupZoneDepent(qreal scale = 1);

    int type() { return QGraphicsItemCustomRect; }

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    int mMinEdgeSize;
    Zone *mZone;
    QString mTitle;
    QStaticText mStaticText;

    //
    qreal mLastScale;
    qreal mBorderWidth;
};

#endif // ZONERECTITEM_H
