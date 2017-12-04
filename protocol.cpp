#include "protocol.h"

ProtocolMsg::ProtocolMsg(QObject *parent)
{
    memset(this->mSetCmd,0,43);
    this->SetMsgSet(this->mSetCmd);
    this->SetMsgMain(this->mMainCmd);



    //测试用***
    /***
    TextCmd(this->setcmd);
    for(int i = 0; i< 11;i++)
    {
        qDebug("%X",setcmd[i]);
    }
    */

}

void ProtocolMsg::SetMsgSet(char m_set[])
{
    Msg msg;
    char cmd[] = "page set";

    //填充 该cmd的body
    /*msg.msg_body.body_type = MSG_BODY_MAIN_TYPE;
    msg.msg_body.body_id = MSG_BODY_MAIN_ID;
    msg.msg_body.body_string = cmd;
    msg.msg_body.body_end_string = HMI_END_STRING;
    msg.msg_body.body_stringLength = strlen(msg.msg_body.body_string) + strlen(msg.msg_body.body_end_string);
    msg.msg_body.body_length = 19 + msg.msg_body.body_stringLength;*/


    qDebug()<<"SetMsgSet";
    //设置msg消息体
    this->SetMsgBody(m_set,cmd);
    //设置msg头
    qDebug()<<"Header";
    this->SetMsgHeader(m_set,strlen(cmd)+19);
    for(int i = 0;i<43;i++)
        qDebug("%x",m_set[i]);

    //header + body;
//    this->m_main = head + body;

}

void ProtocolMsg::SetMsgMain(char m_main[])
{
    char maincmd[] = "page userMain";
    this->SetMsgBody(m_main,maincmd);//body
    this->SetMsgHeader(m_main,strlen(maincmd)+19);//header
    qDebug()<<"Main";
    for(int i = 0;i<48;i++)
        qDebug("%x",m_main[i]);
}


void ProtocolMsg::SetMsgBody(char *body, char *cmd)
{


    char tmp[4];
    memset(tmp,0,4);

    intToByteHex(19+strlen(cmd),tmp);//length
//    qDebug(" Change Over Hex:%X %X %X %X ",tmp[0],tmp[1],tmp[2],tmp[3]);

//    strcpy(body,tmp);m_set 16 -43
    for(int i = 16;i<20;i++)
    {
        body[i] = tmp[i-16];
    }
    qDebug()<<"1";

    intToByteHex(0,tmp);//type
     for(int i = 20;i<24;i++)
     {
         body[i] = tmp[i-20];
     }
    qDebug()<<"2";

     intToByteHex(0,tmp);// id
     for(int i = 24;i<28;i++)
     {
         body[i] = tmp[i-24];
     }
    qDebug()<<"3";

     intToByteHex(strlen(cmd),tmp);// string length
     for(int i = 28;i<32;i++)
     {
         body[i] = tmp[i-28];
     }
    qDebug()<<"4";

     //cmd
     for(int i = 32;i<32+strlen(cmd);i++)
     {
         body[i] = cmd[i-32];
     }
    qDebug()<<"5";

     //cmd_end
     for(int i = 32+strlen(cmd);i<43;i++)
     {
         body[i] = 0xff;
     }
     qDebug()<<"6";


    for(int i = 16; i< 43;i++)
    {
        qDebug("%X",body[i]);
    }
    qDebug()<<"7";
}

void ProtocolMsg::SetMsgHeader(char *head, int msgbodylength)
{
    qDebug()<<"LENGTH:"<<msgbodylength;
     char tmp[4];
     memset(tmp,0,4);
     //version
     intToByteHex(1,tmp);//
     for(int i = 0;i<4;i++)
     {
         head[i] = tmp[i];
     }

     //server
     intToByteHex(1,tmp);//
     for(int i = 4;i<8;i++)
     {
         head[i] = tmp[i-4];
     }

     //version
     intToByteHex(1,tmp);//
     for(int i = 8;i<12;i++)
     {
         head[i] = tmp[i-8];
     }

     //version
     intToByteHex(43,tmp);//
     for(int i = 12;i<16;i++)
     {
         head[i] = tmp[i-12];
     }

     for(int i = 0; i< 16;i++)
     {
         qDebug("%X",head[i]);
     }


//       head =

}



/*
 * int position: 是为解析消息方便引入的  解析个字段到strings  四个字节转一个int
*/
//int protocol::byteHexToInt(QByteArray arry, int postion)//big 大端 网络字节序
//{
//    int value;
//    value = (int)((arry[postion+3] & 0xFF)
//        | ((arry[postion + 2] << 8) & 0xFF00)
//        | ((arry[postion + 1] << 16) & 0xFF0000)
//        | ((arry[postion + 0] << 24) & 0xFF000000));
//    return value;
//}



void ProtocolMsg::intToByteHex(int number,char *abyte0)//大端 网络字节序
{
//    char abyte0[4];
    abyte0[3] = (uchar)  (0x000000ff & number);
    abyte0[2] = (uchar) ((0x0000ff00 & number) >> 8);
    abyte0[1] = (uchar) ((0x00ff0000 & number) >> 16);
    abyte0[0] = (uchar) ((0xff000000 & number) >> 24);//

}


void ProtocolMsg::TextCmd(char setcmd[])
{
    char cmd[] = "page set";
    for(int i = 0;i<8;i++)
    {
        setcmd[i] = cmd[i];
    }

    for(int i = 8;i<11;i++)
    {
        setcmd[i] = 0xff;
    }
}


/**
* @brief             解析Msg
* @param   char msg*         一条序列化的消息
* @param   int msgLength    消息长度  （固定 包括头和消息体）
*
* @return            No
*/
void ProtocolMsg::analysis_Msg(char msg[],int msgLength)
{
    Msg analysis;
    qDebug()<<QString::fromLocal8Bit("解析");
    qDebug("%x",msg[32]);
    int bodyLength = msgLength - 16;
    int cmdLength = bodyLength - 16;
    qDebug()<<msgLength<<bodyLength<<cmdLength;

    char *cmd = (char *)malloc(sizeof(cmdLength));
    //解析命令
    for(int i = 0;i<cmdLength;i++)
        cmd[i] = msg[32+i];

    for(int i = 0;i<cmdLength;i++)
        qDebug("%x",cmd[i]);

    analysis.msg_body.body_string = cmd;

    for(int i = 0;i<cmdLength;i++)
    qDebug("MMMM%x",(analysis.msg_body.body_string)[i]);

//    free(cmd);
}




void ProtocolMsg::fromMainWindow(QString recv,QMutex *mutex,int &fCount)
{
    QMutexLocker Locker(mutex);
    qDebug()<<QString::fromLocal8Bit("自动加锁");

    qDebug()<<QString::fromLocal8Bit("数据:")<<recv;
    char *recvCh_Ptr;
    QByteArray recvByteArray;
    qstringToChar(recv,&recvByteArray,&recvCh_Ptr);

    for(int i = 0;i<recv.size();i++)
    {
        qDebug()<<"enqueue"<<recvCh_Ptr[i];
        recvQueue.enqueue(recvCh_Ptr[i]);
    }

//    foreach (char ch, recvQueue)
//    {
//        qDebug("遍历%c",ch);
//    }
    qDebug()<<QString::fromLocal8Bit("队列大小:")<<recvQueue.size()<<QString::fromLocal8Bit("fCount大小是:")<<fCount;


    //解析
//    while(!recvQueue.isEmpty())//不为空
//    {
//        char oneCh = recvQueue.dequeue();
//        qDebug()<<QString::fromLocal8Bit("出队列:")<<oneCh;
//        oneMsg.append(oneCh);

//        if(oneCh == 'f')
//        {
//            fCount += 1;
//        }
//        if(fCount == 6)
//        {
//            qDebug()<<QString::fromLocal8Bit("当前发Count:")<<fCount<<QString::fromLocal8Bit("队列大小:")<<recvQueue.size();
//            qDebug()<<QString::fromLocal8Bit("唤醒:")<<QString(oneMsg);
//            emit toMainWindowOneMsg(oneMsg);
//            oneMsg.clear();
//            qDebug()<<QString::fromLocal8Bit("oneMsg大小大小大小:")<<oneMsg.size();
//            fCount = 0;
//        }
//    }

    //解析版本2
    while(!recvQueue.isEmpty())
    {
//        for(int i = 0; i < 6;i++)
//        char endCh[6] = recvQueue.dequeue();

        char oneCh = recvQueue.dequeue();
        char secondCh = recvQueue.dequeue();
//        char third = recvQueue.dequeue();
        if(oneCh == 'f' && secondCh == 'f')
        {
            oneMsg.append(oneCh);
            oneMsg.append(secondCh);
            fCount += 1;
        }
        else
        {
            oneMsg.append(oneCh);
            oneMsg.append(secondCh);
        }

        if(fCount == 3)
        {
            qDebug()<<QString::fromLocal8Bit("一条消息:")<<oneMsg;
            emit toMainWindowOneMsg(oneMsg);
            oneMsg.clear();
            fCount = 0;
        }
    }


//    analysis_Msg(oneMsg.data(),43);
}


void ProtocolMsg::qstringToChar(QString cString,QByteArray *byte,char **ch_ptr)
{
    *byte = cString.toLatin1();
    *ch_ptr = (*byte).data();
//    qDebug("ch = %s",*ch_ptr);
//    qDebug("address:%x",ch_ptr);
//    qDebug("address:%x",*ch_ptr);
}

void ProtocolMsg::Test()
{

}

