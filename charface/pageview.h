#ifndef PAGEGRAPHICSVIEW_H
#define PAGEGRAPHICSVIEW_H

#include <QGraphicsView>

class PageGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PageGraphicsView(QWidget *parent = 0);
    
    void setZoom(qreal scale) { QGraphicsView::setTransform(QTransform().scale(scale, scale), false); mScale = scale; }
    qreal getZoom() { return mScale; }

protected:
    qreal mScale;
};

#endif // PAGEGRAPHICSVIEW_H
