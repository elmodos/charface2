#include "scannerdialog.h"
#include "ui_scannerdialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QDir>

#define DEVICE_OUTPUT_SEPARATOR "DEVICE_OUTPUT_SEPARATOR"
#define DEVICE_OUTPUT_NEW_LINE "DEVICE_OUTPUT_NEW_LINE"

ScannerDialog::ScannerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScannerDialog)
{
    //
    ui->setupUi(this);
    setWindowTitle("SANE Frontend");
    setWindowIcon(QIcon(":/scanner.png"));

    //
    ui->buttonScan->setIcon(QIcon(":/scanner.png"));
    ui->buttonCancel->setIcon(QIcon::fromTheme("process-stop"));

    //
    ui->comboBoxColorMode->clear();
    ui->comboBoxColorMode->addItem("Lineart");
    ui->comboBoxColorMode->addItem("Monochrome");
    ui->comboBoxColorMode->addItem("Color");
    ui->comboBoxColorMode->setCurrentIndex(2);

    //configure mProcess
    mProcess = new QProcess(this);
    connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    connect(mProcess, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
    connect(mProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));

    //
    ui->labelProgress->setText("Ready");

    //
    mDialogStatus = DS_Inactive;

    //
    mPagesCount = 0;
    refreshPagesCounter();

    //
    mTempPath = "/tmp/cfscanplugin";
}

ScannerDialog::~ScannerDialog()
{
    delete ui;
}

void ScannerDialog::setTempDirectory(const QString &tempDir)
{
    mTempPath = tempDir;
}

int ScannerDialog::exec()
{
    //get device list
    mProcessOutput.clear();
    mProcessOutputError.clear();
    mDialogStatus = DS_GetDevicesList;

    //show icon
    ui->labelIcon->setPixmap(QIcon(":/scanner.png").pixmap(48));

    //hide interactive area
    ui->widgetInteraction->setHidden(true);
    ui->widgetWait->setHidden(false);
    resize(sizeHint());

    //
    QString param = QString("--formatted-device-list=%d") + DEVICE_OUTPUT_SEPARATOR + "%v %m (%t)" + DEVICE_OUTPUT_NEW_LINE;
    mProcess->start("scanimage", QStringList() << param);

    //
    return QDialog::exec();
}

void ScannerDialog::readyReadStandardOutput()
{
    //append received data

    mProcessOutput.append(mProcess->readAllStandardOutput());
}

void ScannerDialog::readyReadStandardError()
{
    if (mDialogStatus == DS_Scan)
    {
        //display scan progress
        QByteArray progress = mProcess->readAllStandardError();
        QString text = QString(progress);
        QStringList list = text.split(" ");

        //"Progress 0.00%"
        if (2 == list.size())
        {
            QString val = list.at(1).split(".").at(0);
            ui->progressBar->setValue(val.toInt());

            ui->labelProgress->setText("Scanning...");
        }
    }
    else
    {
        //append received data
        mProcessOutputError.append(mProcess->readAllStandardError());
    }
}

void ScannerDialog::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    //if process failed
    if ( mDialogStatus == DS_GetDevicesList && (exitCode || exitStatus == QProcess::CrashExit) )
    {
        //
        QMessageBox messageBox(QMessageBox::Critical, "Oops!", "Abnormal \"scanimage\" process exit status.", QMessageBox::Close);
        messageBox.setDetailedText(QString(mProcessOutputError));
        qDebug() << "Error message: " << QString(mProcessOutputError);
        //
        qDebug() << mProcessOutputError;

        mProcessOutputError.clear();
        mProcessOutput.clear();

        //
        messageBox.exec();
        this->reject();
    }

    //
    switch (mDialogStatus)
    {
        case DS_GetDevicesList:
        {
            //
            mDialogStatus = DS_Inactive;
            qDebug() << mProcessOutput;

            //update combobox
            QStringList list = QString(mProcessOutput).split(DEVICE_OUTPUT_NEW_LINE);
            refreshDevicesList(list);

            //show interactive area
            ui->labelIcon->setText("");
            ui->widgetInteraction->setHidden(false);
            ui->widgetWait->setHidden(true);
            resize(sizeHint());

            break;
        }

        case DS_Scan:
        {
            qDebug() << "Image scanned, enabling buttons";

            //
            mDialogStatus = DS_Inactive;
            ui->buttonScan->setEnabled(true);
            ui->buttonCancel->setEnabled(false);

            //
            ui->labelProgress->setText("Ready");
            ui->progressBar->setValue(100);

            //
            if ( ! (exitCode || exitStatus == QProcess::CrashExit) )
            {
                //get file
                QDir dir(mTempPath);
                dir.mkpath(mTempPath);
                QString fileName = QString::number(mPagesCount) + ".tiff";
                QFile file(dir.absoluteFilePath(fileName));

                //open
                if (file.open(QIODevice::WriteOnly))
                {
                    //write
                    file.write(mProcessOutput);
                    mProcessOutput.clear();

                    // this file will be used
                    mFiles.append(file.fileName());

                    //
                    mPagesCount++;

                    //
                    refreshPagesCounter();
                }
                else
                {
                    QMessageBox messageBox(QMessageBox::Critical, "Error", "Error writing to file.", QMessageBox::Close);
                    messageBox.setDetailedText(file.errorString());
                    messageBox.exec();
                }
            }
            else
            {
                QMessageBox messageBox(QMessageBox::Critical, "Oops!", "Abnormal \"scanimage\" process exit status.", QMessageBox::Close);
                messageBox.exec();
            }

            break;
        }

        default:
            break;
    }
}

void ScannerDialog::onScan()
{
    //
    mDialogStatus = DS_Scan;

    //
    mProcessOutput.clear();
    mProcessOutputError.clear();

    //
    ui->labelProgress->setText("Warm up...");
    ui->buttonScan->setEnabled(false);
    ui->buttonCancel->setEnabled(true);
    ui->progressBar->setValue(0);

    //get device id
    int index = ui->comboBoxDevices->currentIndex();
    QString deviceId = mDevices.at(index);

    //color mode
    QString colorMode = ui->comboBoxColorMode->currentText();

    //
    QStringList params;
    params << QString("--device-name=") + deviceId;
    params << "--format=tiff";
    params << "--progress";
    params << QString("--mode");
    params << colorMode;

    //
    mProcess->start("scanimage", params);
}

void ScannerDialog::onCancel()
{
    mProcess->terminate();
    processFinished(1, QProcess::NormalExit);
}

void ScannerDialog::refreshDevicesList(const QStringList &list)
{
    //show icon
    ui->labelIconScanner->setPixmap(QIcon(":/scanner.png").pixmap(48));

    //clear current device ID list
    mDevices.clear();
    ui->comboBoxDevices->clear();

    //add items to device list and combo box
    foreach (QString line, list)
    {
        //
        QStringList l = line.split(DEVICE_OUTPUT_SEPARATOR);

        //skip invalid format
        if (l.size() != 2)
            continue;

        //add strings
        mDevices.append(l.at(0));
        ui->comboBoxDevices->addItem(l.at(1));
    }
}

void ScannerDialog::refreshPagesCounter()
{
    ui->labelPageCounter->setText(QString("Pages scanned %1").arg(mPagesCount));
}

