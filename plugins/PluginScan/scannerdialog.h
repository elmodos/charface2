#ifndef SCANNERDIALOG_H
#define SCANNERDIALOG_H

#include <QtWidgets/QDialog>
#include <QProcess>
#include <QStringList>

namespace Ui {
class ScannerDialog;
}

class ScannerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ScannerDialog(QWidget *parent = 0);
    ~ScannerDialog();

    void setTempDirectory(const QString &tempDir);
    int exec();
    QStringList files() { return mFiles; }

private slots:
    void readyReadStandardOutput();
    void readyReadStandardError();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onScan();
    void onCancel();

private:
    void refreshDevicesList(const QStringList &list);
    void refreshPagesCounter();

    Ui::ScannerDialog *ui;
    QProcess *mProcess;
    QByteArray mProcessOutput;
    QByteArray mProcessOutputError;
    QStringList mDevices;
    QStringList mFiles;
    QString mTempPath;

    enum DialogStatus
    {
        DS_Inactive = 0,
        DS_GetDevicesList,
        DS_Scan
    };
    DialogStatus mDialogStatus;

    int mPagesCount;

};

#endif // SCANNERDIALOG_H
