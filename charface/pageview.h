#ifndef PAGEGRAPHICSVIEW_H
#define PAGEGRAPHICSVIEW_H

#include <QGraphicsView>

class PageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PageView(QWidget *parent = 0);
    
    void setZoom(qreal scale);
    qreal getZoom() const { return mScale; }

protected:
    qreal mScale;
};

#endif // PAGEGRAPHICSVIEW_H
