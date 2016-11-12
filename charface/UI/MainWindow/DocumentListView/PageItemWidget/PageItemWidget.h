#ifndef PAGEITEMWIDGET_H
#define PAGEITEMWIDGET_H

#include <QWidget>

namespace Ui {
class ScrollItem;
}

class PageItemWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PageItemWidget(QWidget *parent = 0);
    ~PageItemWidget();
    
    static int defaultHeight();

    void setPixmap(const QPixmap &pixmap);
    void setTitle(const QString &title);

    void setHighlighted(bool val);
    bool isHighlighted() { return mIsHighlighted; }

    void setOverlayWidget(QWidget *widget) { mOverlayWidget = widget; }

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    Ui::ScrollItem *ui;
    static int allocatedcount;
    bool mIsHighlighted;
    QWidget *mOverlayWidget;
};

#endif // PAGEITEMWIDGET_H
