#include "recvthread.h"
#include <QDebug>

recvThread::recvThread(QObject *parent)
    : QThread(parent)
{

}

void recvThread::run()
{

}

void recvThread::readFormSerial()
{
//   qDebug()<<QString::fromLocal8Bit("读取:")<<port.readAll();
    qDebug()<<QString::fromLocal8Bit("串口可用数据:")<<port->bytesAvailable();
    QString recv = port->readAll().toHex();
    emit toMainWindowForSendRecv(recv);

//    if(dataQueue.at(i))
//    qDebug()<<QString::fromLocal8Bit("当前接受长度:")<<account;
//    qDebug()<<QString::fromLocal8Bit("串口返回数据:")<<recv;
//        qDebug()<<QString::fromLocal8Bit("D")<<dataQueue.at(1);
//   ui->textEdit->append(recv);
}
