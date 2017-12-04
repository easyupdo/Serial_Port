/********************************************************
 *  @file           protocl.h
 *  @brief         no
 *  @details      no
 *  @author      Jay
 *  @emile        no
 *  @version     0.1
 *  @date        2017-11-28
********************************************************/


#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <string>
#include <QDebug>
#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <QQueue>
#include <QMutexLocker>
#include <QMutex>

using namespace std;
//消息头
#define MSG_HEADER_VERSION 0.1
#define MSG_HEADER_MASK 0
#define MSG_HEADER_SERVER 1

//消息体
#define MSG_BODY_MAIN_TYPE 1
#define MSG_BODY_MAIN_ID 1

//HMI命令结束标志
#define HMI_END_STRING "0xff0xff0xff"

//消息body开始位置
#define BODY_START_POSITION 16
/**
* @brief 协议头结构  包括协议版本号 协议复用服务号 协议掩码 消息总长度
*/
typedef unsigned char uchar;

typedef struct HadiMsgHeader
{
    //协议头定长 16byte
   //协议头版本号
    int version;
    //协议头服务复用号码
    int server;
    //协议掩码
    int mask;
    //协议长度
    int length;/** 协议头+消息体 总长度 */
}MsgHeader;



/**
* @brief  协议消息体  包括  消息体长度  消息类型  消息id  消息指令长度  消息指令内容
* 消息指令内容 变长
*/
typedef struct HadiMsgBody
{
    ///一条消息体长度                     4byte
    int body_length;
    ///消息类型                             4byte
    int body_type;
    ///消息id                                4byte
    int body_id;
    ///消息指令长度                        4byte
    int body_stringLength;
    ///消息指令内容       变长
    char * body_string;
    ///消息指令结束标志0xff0xff0xff    3byte
    char * body_end_string;

}MsgBody;


typedef struct Msg
{
    MsgHeader msg_header;
    MsgBody msg_body;
}Msg;



/**
*  @brief HMI与device之间指令传送协议
*  类的详细概述
*/

class ProtocolMsg:public QObject
{
    Q_OBJECT
public:
    ProtocolMsg(QObject * parent = 0);
    ///Generate a message
    /**
    * @brief              当屏幕点击main时 生成跳转main界面的指令消息
    *
    * @return            生成跳转mian界面的消息
    */
    void SetMsgMain(char m_main[]);
    void SetMsgSet(char m_set[]);
    void SetMsgButton();
    void SetMsgListen();
    void SetMsgDefault();
    void SetMsgPlayList();
    void SetMsgSearchPlay();
    void SetMsgIpInput();
    void SetMsgKeyInput();
    void SetMsgPortInput();

    void TextCmd(char setcmd[]);

    /**
    * @brief              设置一条消息消息头
    *
    * @return            no
    */
    void SetMsgHeader(char *head, int msgbodylength);

    void SetMsgBody(char *body, char *cmd);

    void intToByteHex(int number, char *tmp);

    void qstringToChar(QString cString,QByteArray *byte,char **ch_ptr);
    void analysis_Msg(char msg[], int msgLength);


    void Test();

signals:
    void toMainWindowOneMsg(QByteArray);
public slots:
    void fromMainWindow(QString, QMutex *mutex, int &fCount);

private:
    Msg * msg;

public:
    ///MSG
    char mMainCmd[48];/**跳转main界面消息 16 + 19 + "page userMain"*/
    char mSetCmd[43];/**跳转到set界面消息*/
//    char mButton[];
    char mListenCmd[46];/**跳转到 listen界面消息 16 + 19 + page listen*/
    char mDefaultCmd[47];/**跳转到default界面消息 16+19+page default*/
    char mPlayListCmd[48];/**跳转到playlist界面消息*/
    char mSearchPlayCmd[50];
    char mIpInputCmd[47];
    char mKeyInputCmd[48];
    char mPortInputCmd[49];

    QQueue<char> recvQueue;

    QByteArray oneMsg;

    QMutex *mutex;


};

#endif /// PROTOCOL_H
