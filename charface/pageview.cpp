#include "pageview.h"

PageView::PageView(QWidget *parent) :
    QGraphicsView(parent)
{
    mScale = 1;
}

void PageView::setZoom(qreal scale)
{
    QGraphicsView::setTransform(QTransform().scale(scale, scale), false);
    mScale = scale;
}
