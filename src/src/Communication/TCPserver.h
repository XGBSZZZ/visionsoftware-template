#pragma once

#include "Useme.h"
#include <QMainWindow>
#include  <QTcpServer>
#include "qtimer.h"
#include "ui_TCPserver.h"

namespace Ui {
class MainWindow;
}

class TCPServeronlyone : public QMainWindow
{
    Q_OBJECT

private:

     QTcpServer* tcpServer; //TCP服务器  用于监听

    QVector<QTcpSocket*> tcpSockets;
    QString getLocalIP();//获取本机IP地址
    QTimer* m_heartbeat = new QTimer;

    bool connectionisok = false;
    int nowprice = 0;
    int clientnumb = 0;
private:
   // void closeEvent(QCloseEvent *event);
    void show_localtext(const QString&);

public:
    explicit TCPServeronlyone(QWidget *parent = 0);
    ~TCPServeronlyone();

    bool isconnect(){ return connectionisok;}

    void SendData(const QString& z)  ; //******  发消息

    void Start() { on_actStart_triggered(); }

private slots:
    //蛙达西赛高你还铁牙自大
    void onNewConnection();//且让我绑定一下
    void onClientConnected(); //客户端接入时 文本框的显示
    void onClientDisconnected();  //客户端断开连接时 文本框的显示以及 断开连接
    void onSocketReadyRead();//******收消息
    void on_actStart_triggered();//开始监听按钮
    void on_actStop_triggered();//停止通讯按钮
    void on_actClear_triggered();//清空文本框按钮
    void on_btnSend_clicked(); //发送信息测试按钮  发送一行字符串，以换行符结束
    //void on_actHostInfo_triggered();//获取本机地址
    void on_use_angelbeats(int);  //是否启用天使の心跳
    void on_angelbeats();  //天使の心跳
    void on_angelbeats_timeer_change(int );  //天使の心跳频率改变

private:
    Ui::MainWindow *ui;

signals:
  void  sig_ReceivedData(const QString &);
  void  sig_Communicationstate(const bool&);
};


