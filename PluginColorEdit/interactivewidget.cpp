#include "interactivewidget.h"
#include "ui_interactivewidget.h"

InteractiveWidget::InteractiveWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InteractiveWidget)
{
    ui->setupUi(this);
}

InteractiveWidget::~InteractiveWidget()
{
    delete ui;
}
