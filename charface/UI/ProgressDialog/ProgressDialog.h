#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();

    //
    void setCancelButtonVisible(bool visible);

    //label text
    void setText(const QString &text);

    //progress bar bounds
    void setMax(int maxVal);
    void setVal(int val);

    //progress bar title text format
    void setProgressFormat(const QString &format);

    //set up the message filter to be treatet as no-error
    void setCanCloseWindowForMessageTypes(int mask) { mIgnoreMask = mask; }

    //try to hide and delete this dialog if no out-of-filter messages are present
    void closeIfNoMessages();

    //log message icon/type
    enum  {
        MI_None    = 1,
        MI_Info    = 1 << 1,
        MI_Warning = 1 << 2,
        MI_Error   = 1 << 3
    };

    //append message, will show log box if was not visible
    void addMessage(const QString &message, int icon = MI_None);

signals:
    void cancelled();
    void cancelClicked();

private slots:
    void onCancel();
    void onClose();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::ProgressDialog *ui;

    //
    int mIgnoreMask;
    bool mOutOfMask;
    bool mCanClose;
    bool mCancelClicked1stTime;
};

#endif // PROGRESSDIALOG_H
