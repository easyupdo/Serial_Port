#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    openOrClosePushButton = false;
    fCount = 0;
    recAsciiOrHexControl = AsciiOrHex::ASCII;
    this->setWindowTitle(QString::fromLocal8Bit("串口调试助手"));
    ui->statusBar->addWidget(new QLabel(QString::fromLocal8Bit("串口信息:")));
    ui->statusBar->addWidget(&connectStatus);
    ui->statusBar->addWidget(&connectSeriaPortInfo);
    initSerialPortInfo();
    portRedvThread = new recvThread;
//    this->moveToThread(portRedvThread);//丢进新线程执行
    portRedvThread->start();
    portRedvThread->port = &port;

    connect(&port,&QSerialPort::readyRead,portRedvThread,&recvThread::readFormSerial,Qt::QueuedConnection);
    connect(portRedvThread,&recvThread::toMainWindowForSendRecv,this,&MainWindow::fromRecvThreadForRecv);
    connect(this,&MainWindow::toProtocol,&protocolmsg,&ProtocolMsg::fromMainWindow);//发送给protocol 入队列 解析

    connect(&protocolmsg,&ProtocolMsg::toMainWindowOneMsg,this,&MainWindow::fromProtocolMsgOneMsg);

    //    connect(&time,&QTimer::timeout,this,&MainWindow::writeToHMI_SetTime);


    //用语串口测试新协议




}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::writeToSerial()
{
    port.clear();
    if(recAsciiOrHexControl == AsciiOrHex::ASCII)
    {
        QString string = "page set";
//        const char over[] = {0xff,0xff,0xff};
        QByteArray flag;
        flag.append(0xff);
        flag.append(0xff);
        flag.append(0xff);
        QByteArray use = string.toLatin1()+flag;
        toHMI = use;
        qDebug()<<"USE:"<<use;
        qDebug()<<"USE To Hex:"<<use.toHex();
        port.write(use);
//        port.clear();
    }
    if(recAsciiOrHexControl == AsciiOrHex::HEX)
    {
        QString string = ui->textEdit_2->toPlainText();
        QByteArray use = string.toLatin1();
        qDebug()<<QString::fromLocal8Bit("HEX:")<<string<<use;
        port.write(use);
    }

}

void MainWindow::on_pushButton_clicked()
{
    writeToSerial();


}

void MainWindow::on_pushButton_2_clicked()
{

    openOrClosePushButton = !openOrClosePushButton;
    if(openOrClosePushButton)
    {
        QSerialPortInfo onePortInfo;
        qint32 userBaudRate;
        QSerialPort::DataBits userDataBits;
        QSerialPort::Parity userParity;
        QSerialPort::StopBits userStopBits;
        QSerialPort::FlowControl userFlowControl;
        //端口
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            if(info.portName() == ui->comboBox->currentText())
            {
                onePortInfo = info;
                qDebug()<<QString::fromLocal8Bit("选择串口:")<<onePortInfo.portName();
            }
        }

        //波特率
        if(ui->comboBox_2->currentText() == "9600")
             userBaudRate = QSerialPort::Baud9600;
        else if(ui->comboBox_2->currentText() == "19200")
            userBaudRate = QSerialPort::Baud19200;
        else if(ui->comboBox_2->currentText() == "38400")
            userBaudRate = QSerialPort::Baud38400;
        else if(ui->comboBox_2->currentText() == "57600")
            userBaudRate = QSerialPort::Baud57600;
        else if(ui->comboBox_2->currentText() == "115200")
            userBaudRate = QSerialPort::Baud115200;

        //数据位
        if(ui->comboBox_3->currentText() == "5")
            userDataBits = QSerialPort::Data5;
        else if(ui->comboBox_3->currentText() == "6")
            userDataBits = QSerialPort::Data6;
        else if(ui->comboBox_3->currentText() == "7")
            userDataBits = QSerialPort::Data7;
        else if(ui->comboBox_3->currentText() == "8")
            userDataBits = QSerialPort::Data8;

        //校验位
        if(ui->comboBox_4->currentText() == "None")
            userParity = QSerialPort::NoParity;
        else if(ui->comboBox_4->currentText() == "Even")
            userParity = QSerialPort::EvenParity;
        else if(ui->comboBox_4->currentText() == "Odd")
            userParity = QSerialPort::OddParity;
        else if(ui->comboBox_4->currentText() == "Mark")
            userParity = QSerialPort::MarkParity;
        else if(ui->comboBox_4->currentText() == "Space")
            userParity = QSerialPort::SpaceParity;

        //停止位
        if(ui->comboBox_5->currentText() == "1")
            userStopBits = QSerialPort::OneStop;
        else if(ui->comboBox_5->currentText() == "1.5")
            userStopBits = QSerialPort::OneAndHalfStop;
        else if(ui->comboBox_5->currentText() == "2")
            userStopBits = QSerialPort::TwoStop;

        //流控
        if(ui->comboBox_6->currentText() == "None")
            userFlowControl = QSerialPort::NoFlowControl;
        else if(ui->comboBox_6->currentText() == "RTS/CTS")
            userFlowControl = QSerialPort::HardwareControl;
        else if(ui->comboBox_6->currentText() == "XON/XOFF")
            userFlowControl = QSerialPort::SoftwareControl;

        //填充到端口
        {
            port.setPort(onePortInfo);
            port.setBaudRate(userBaudRate);
            port.setDataBits(userDataBits);
            port.setParity(userParity);
            port.setStopBits(userStopBits);
            port.setFlowControl(userFlowControl);
        }
        port.clearError();

        int ret = port.open(QIODevice::ReadWrite);
        if(ret)
        {
            qDebug()<<QString::fromLocal8Bit("打开串口成功!");
            QString connectSerialPortInfoString = port.portName() +" "+ QString::number(port.baudRate()) + " " +ui->comboBox_3->currentText()+" "+ ui->comboBox_4->currentText()+" "+ui->comboBox_5->currentText()+" "+ui->comboBox_6->currentText();
            this->connectStatus.setText(QString::fromLocal8Bit("连接  "));
            this->connectSeriaPortInfo.setText(connectSerialPortInfoString);

            {
                //连接上了 所有不可选
                ui->comboBox->setEnabled(false);
                ui->comboBox_2->setEnabled(false);
                ui->comboBox_3->setEnabled(false);
                ui->comboBox_4->setEnabled(false);
                ui->comboBox_5->setEnabled(false);
                ui->comboBox_6->setEnabled(false);
                ui->pushButton_2->setText(QString::fromLocal8Bit("关闭"));
            }

            time.start(1000);
        }
        else
        {

            QString errorString = port.errorString();
            qDebug()<<errorString;
            this->connectStatus.setText(QString::fromLocal8Bit("断开"));
            this->connectSeriaPortInfo.setText(errorString);
        }
    }
    else
    {
        port.clear();
        port.close();
        {
            //连接上了 所有不可选
            ui->comboBox->setEnabled(true);
            ui->comboBox_2->setEnabled(true);
            ui->comboBox_3->setEnabled(true);
            ui->comboBox_4->setEnabled(true);
            ui->comboBox_5->setEnabled(true);
            ui->comboBox_6->setEnabled(true);
            ui->pushButton_2->setText(QString::fromLocal8Bit("连接"));
        }
        this->connectStatus.setText(QString::fromLocal8Bit("断开"));
        this->connectSeriaPortInfo.setText("");

    }

}


void MainWindow::initSerialPortInfo()
{
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    ui->comboBox_5->clear();
    ui->comboBox_6->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(info.portName());
    }
    QList<QString>items_ComboBox_2;
    items_ComboBox_2<<"9600"<<"19200"<<"38400"<<"57600"<<"115200";
    ui->comboBox_2->addItems(items_ComboBox_2);
    QList<QString>items_ComboBox_3;
    items_ComboBox_3<<"5"<<"6"<<"7"<<"8";
    ui->comboBox_3->addItems(items_ComboBox_3);
    ui->comboBox_3->setCurrentIndex(3);
    QList<QString>items_ComboBox_4;
    items_ComboBox_4<<"None"<<"Even"<<"Odd"<<"Mark"<<"Space";
    ui->comboBox_4->addItems(items_ComboBox_4);
    QList<QString>items_ComboBox_5;
    items_ComboBox_5<<"1"<<"1.5"<<"2";
    ui->comboBox_5->addItems(items_ComboBox_5);
    QList<QString>items_ComboBox_6;
    items_ComboBox_6<<"None"<<"RTS/CTS"<<"XON/XOFF";
    ui->comboBox_6->addItems(items_ComboBox_6);
}



void MainWindow::on_radioButton_4_clicked()
{
    recAsciiOrHexControl = AsciiOrHex::HEX;
}

void MainWindow::on_radioButton_3_clicked()
{
    recAsciiOrHexControl = AsciiOrHex::ASCII;
}


void MainWindow::StringToHex(QString str, QByteArray &senddata) //字符串转换为十六进制数据0-F
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;

    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return ch-ch;//不在0-f范围内的会发送成0
}

void MainWindow::writeToHMI_SetTime()
{
//    writeToSerial();
//    qDebug()<<QString::fromLocal8Bit("写入HMI:")<<toHMI;
    QString string = "n0.val=n0.val+1";
//    char over[] = {0xff,0xff,0xff};
    QByteArray flag;
    flag.append(0xff);
    flag.append(0xff);
    flag.append(0xff);
    qDebug()<<"flag:"<<flag;
    QByteArray use;
    use.clear();
    use = string.toLatin1()+flag;
    qDebug()<<"TIME:"<<use;
    port.write(use);
}

//test  设置
void MainWindow::on_pushButton_3_clicked()
{
    QString string = "page set";
//    char over[] = {0xff,0xff,0xff};
    QByteArray flag;
    flag.append(0xff);
    flag.append(0xff);
    flag.append(0xff);
    QByteArray use;
    use.clear();
    use = string.toLatin1()+flag;
    qDebug()<<"USE:"<<use;
    port.write(use);
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->textEdit->clear();
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->textEdit_2->clear();
}

void MainWindow::on_pushButton_6_clicked()
{
    port.write(this->protocolmsg.mSetCmd);
}




/**
    * @brief              将接受到的数据添加到队列中    ok
    * @param index    QByteArray 变量 QString 转 char 中间变量
    * @param t          char 指针 用于添加队列时用
    *
    * @return            NO
    */
void MainWindow::fromRecvThreadForRecv(QString recv)
{
    //送去protocol加入队列 并解析
    {
       emit toProtocol(recv,&mutex,fCount);
    }
}


void MainWindow::fromProtocolMsgOneMsg(QByteArray oneMsg)
{
    ui->textEdit->append(oneMsg);
}




