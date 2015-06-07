#include <QDebug>

#include "progressdialog.h"
#include "ui_progressdialog.h"
#include "utilities.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    ui->widgetDetails->setHidden(true);
    ui->buttonClose->setHidden(true);

    //
    mIgnoreMask = 0;
    mOutOfMask = false;
    mCanClose = false;
    mCancelClicked1stTime = true;

    //resize, keep desiger width
    QSize size = this->size();
    resize( size.width(), sizeHint().height() );

    //
    setCanCloseWindowForMessageTypes(ProgressDialog::MI_Info);
    setAttribute( Qt::WA_DeleteOnClose );
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::setCancelButtonVisible(bool visible)
{
    ui->buttonCancel->setVisible(visible);
}

void ProgressDialog::setText(const QString &text)
{
    ui->label->setText(text);
}

void ProgressDialog::setMax(int maxVal)
{
    ui->progressBar->setMaximum(maxVal);
}

void ProgressDialog::setVal(int val)
{
    ui->progressBar->setValue(val);
}

void ProgressDialog::setProgressFormat(const QString &format)
{
    ui->progressBar->setFormat(format);
}

void ProgressDialog::closeIfNoMessages()
{
    mCanClose = true;
    ui->buttonClose->setHidden( !mOutOfMask );

    ui->buttonCancel->setVisible(false);

    if ( !mOutOfMask)
        close();
}

void ProgressDialog::addMessage(const QString &message, int icon)
{
    if( ui->widgetDetails->isHidden() )
    {
        ui->widgetDetails->setHidden(false);

        //resize, keep desiger width
        QSize size = this->size();
        resize( size.width(), sizeHint().height() );
    }

    QString iconName = "";
    switch (icon)
    {
        case MI_None:    iconName = "";               break;
        case MI_Error:   iconName = "error";          break;
        case MI_Info:    iconName = "info";           break;
        case MI_Warning: iconName = "dialog-warning"; break;
        default: break;
    }
    QListWidgetItem *item = new QListWidgetItem(LOAD_ICON(iconName), message);
    ui->listWidget->addItem(item);
    ui->listWidget->scrollToBottom();

    qDebug() << "New message" << icon << "out of filter (before)" << mOutOfMask;
    qDebug() << "is in mask" << ((mIgnoreMask & icon) == icon);
    mOutOfMask |= ( (mIgnoreMask & icon) != icon);
    qDebug() << "out of filter (after)" << mIgnoreMask;
}

void ProgressDialog::closeEvent(QCloseEvent *event)
{
    qDebug() << "Close event";
    if( mCanClose )
        event->accept();
    else
        event->ignore();        
}


void ProgressDialog::onCancel()
{
    //on any cancel click
    emit cancelClicked();

    //only 1st click
    if (mCancelClicked1stTime)
        emit cancelled();

    //store state
    mCancelClicked1stTime = false;
}

void ProgressDialog::onClose()
{
    close();
}
