#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include  <QtSerialPort>
#include <QtSerialPortVersion>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QByteArray>
#include <QLabel>
#include <QTimer>
#include <QThread>
#include "recvthread.h"
#include "protocol.h"
#include <QMutex>
enum AsciiOrHex
{
    ASCII,
    HEX,
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initSerialPortInfo();
    void StringToHex(QString str, QByteArray &senddata);
    char ConvertHexChar(char ch);
    void writeToHMI_SetTime();


    void qstringToChar(QString cString,QByteArray *byte,char **ch_ptr);


signals:
    void toProtocol(QString,QMutex *,int &);


public slots:
    void fromRecvThreadForRecv(QString);

    void fromProtocolMsgOneMsg(QByteArray oneMsg);

private slots:
    void writeToSerial();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_3_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPortInfo portInfo;
    QSerialPort port;
    QLabel connectStatus;
    QLabel connectSeriaPortInfo;
    bool openOrClosePushButton;
    bool recAsciiOrHexControl;

    QTimer time;
    QByteArray toHMI;
    recvThread *portRedvThread;


    ProtocolMsg protocolmsg;
    QMutex mutex;

    int fCount;


};

#endif // MAINWINDOW_H
