#ifndef PAGEGRAPHICSSCENE_H
#define PAGEGRAPHICSSCENE_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "page.h"
#include "zonerectitem.h"

enum {
    MS_None,

    MS_Move,

    MS_Top,
    MS_Bottom,
    MS_Left,
    MS_Right,

    MS_TopLeft,
    MS_TopRight,
    MS_BottomLeft,
    MS_BottomRight
};
typedef int MoveState;

class PageGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PageGraphicsScene(QWidget *parent = 0);
    
    bool loadPage(Page *page);
    Page *page() { return mPage; }
    QGraphicsPixmapItem *pageItem() { return mPixmapItem; }

    //
    qreal scaleInView() const { return mScaleInView; }
    void setScaleInView(const qreal val) { mScaleInView = val; }

    //
    QWidget *scalableView() const { return mScalableView; }
    void seteScalableView(QWidget *gv) { mScalableView = gv; }

    //scale
    qreal getZoomScale() const { return mScaleInView; }

    //
    void setZonePenType(ZoneType zoneType) { mZonePenType = zoneType; }
    ZoneType zonePenType() { return mZonePenType; }

    //
    bool deleteWithMouseClick() { return mDeleteWithMouseClick; }
    void setDeleteWithMouseClick(bool val) { mDeleteWithMouseClick = val; }

    //
    QSize pageSize() { return QSize(mPageWidth, mPageHeight); }

    //
    void repaintRects();

private slots:
    void onChangeZoneType();
    void onDeleteZone();

signals:
    void pageChanged();

protected:
    //
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    //
    void showContextMenu();

    //
    MoveState getMoveState(qreal x, qreal y, qreal left, qreal top, qreal right, qreal bottom, qreal delta) const;

    //
    void createRects();
    ZoneRectItem *createRect(Zone *zone, int index);

private:
    //
    Page *mPage;
    int mPageWidth, mPageHeight;

    //
    bool mHasImage;
    QGraphicsPixmapItem *mPixmapItem;

    //
    bool mMouseIsDown;
    bool mDeleteWithMouseClick;
    Qt::MouseButton mPressedButton;

    //
    qreal edgeMagnetDistance;

    //
    qreal mScaleInView;
    QWidget *mScalableView;

    //
    ZoneRectItem *mCurrentItem;
    MoveState mMoveState;
    QPointF mMovePos;

    //
    ZoneType mZonePenType;
};

#endif // PAGEGRAPHICSSCENE_H
