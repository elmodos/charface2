#include <QDebug>

#include "PageItemWidget.h"
#include "ui_PageItemWidget.h"

int PageItemWidget::allocatedcount = 0;

PageItemWidget::PageItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScrollItem)
{
    ui->setupUi(this);
    mIsHighlighted = false;
    mOverlayWidget = NULL;

}

PageItemWidget::~PageItemWidget()
{
    if (mOverlayWidget)
    {
        mOverlayWidget->hide();
        mOverlayWidget->setParent(parentWidget());
    }

    delete ui;
}

int PageItemWidget::defaultHeight()
{
    return 170;
}

void PageItemWidget::setPixmap(const QPixmap &pixmap)
{
    ui->labelPixmap->setPixmap(pixmap);
}

void PageItemWidget::setTitle(const QString &title)
{
    ui->labelTitle->setText(title);
}


void PageItemWidget::setHighlighted(bool val)
{
    mIsHighlighted = val;
    setBackgroundRole( (val ? QPalette::Highlight : QPalette::NoRole));
}

void PageItemWidget::enterEvent(QEvent *)
{
    if (!mOverlayWidget) return;
    mOverlayWidget->move(0, (height() - mOverlayWidget->height()) / 2);
    mOverlayWidget->setParent(this);
    mOverlayWidget->show();
}

void PageItemWidget::leaveEvent(QEvent *)
{
    if (!mOverlayWidget) return;
    mOverlayWidget->setParent(parentWidget());
    mOverlayWidget->hide();
}
