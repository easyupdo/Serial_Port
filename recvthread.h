#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QAbstractItemModel>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>

class recvThread : public QThread
{
    Q_OBJECT

public:
    explicit recvThread(QObject *parent = 0);

    void run();

    void readFormSerial();

signals:
    void toMainWindowForSendRecv(QString);

public:
    QSerialPort  *port;


};

#endif // RECVTHREAD_H
