#include "Useme.h"
#include "TCPserver.h"
#include  <QtNetwork>




TCPServeronlyone::TCPServeronlyone(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    tcpServer = new QTcpServer(this);
    ui->btnSend->setEnabled(false);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    //加载保存文件

    
    // 天使の心跳连接信号槽
    m_heartbeat->setInterval(ui->beat_timer->value() * 1000);
    connect(ui->use_angel_beats, SIGNAL(stateChanged(int)), this, SLOT(on_use_angelbeats(int)));
    connect(m_heartbeat, SIGNAL(timeout()), this, SLOT(on_angelbeats()));
    connect(ui->beat_timer, SIGNAL(valueChanged(int)), this, SLOT(on_angelbeats_timeer_change(int)));
}

TCPServeronlyone::~TCPServeronlyone()
{
    if (tcpServer->isListening()) //tcpServer正在监听
    {
        tcpServer->close();//停止监听
        ui->actStart->setEnabled(true);
        ui->actStop->setEnabled(false);
    }

    for (auto& i : tcpSockets)
    {
        i->close();
        i->deleteLater();
    }

    m_heartbeat->stop();
    delete m_heartbeat;
    delete ui;
}


//发信息
void TCPServeronlyone::SendData(const QString& z) 
{
    try
    {
        if (connectionisok)
        {
     //发送一行字符串，以换行符结束
        //QString  msg = ui->editMsg->text();
        ui->editMsg->clear();
        ui->editMsg->setFocus();

       // QByteArray  str = z.toUtf8();
       // str.append('\n');//添加一个换行符

        for (auto &i : tcpSockets)
        {
            i->write((z.toLocal8Bit()).append('\n'));
        }        
        show_localtext(tr("[发送] ") + z);

        }
        else
        {
            show_localtext(tr("发送失败,通讯未连接"));
        }
      
    }
    catch (const std::exception&)
    {
       
    }
    
}

QString TCPServeronlyone::getLocalIP()
{//获取本机IPv4地址
    QString hostName=QHostInfo::localHostName();//本地主机名
    QHostInfo   hostInfo=QHostInfo::fromName(hostName);
    QString   localIP="";

    QList<QHostAddress> addList=hostInfo.addresses();//

    if (!addList.isEmpty())
    for (int i=0;i<addList.count();i++)
    {
        QHostAddress aHost=addList.at(i);
        if (QAbstractSocket::IPv4Protocol==aHost.protocol())
        {
            localIP=aHost.toString();
            break;
        }
    }
    return localIP;
}


////关闭窗口时停止监听
//void TCPServeronlyone::closeEvent(QCloseEvent *event)
//{
//    if (tcpServer->isListening())
//        tcpServer->close();;//停止网络监听
//    event->accept();
//}

void TCPServeronlyone::show_localtext(const QString& data)
{
    ui->plainTextEdit->appendPlainText("[发送] " + data);
}

void TCPServeronlyone::onNewConnection()
{

    QTcpSocket* tcpSocket;//TCP通讯的Socket  //用于接受与发送


    tcpSocket = tcpServer->nextPendingConnection(); //创建socket

    tcpSockets.push_back(tcpSocket);

    connect(tcpSocket, SIGNAL(connected()),     //连接时
            this, SLOT(onClientConnected()));
    onClientConnected();//

    connect(tcpSocket, SIGNAL(disconnected()),  //断开连接时
            this, SLOT(onClientDisconnected()));

    connect(tcpSocket,SIGNAL(readyRead()), //读取缓冲区行文本
            this,SLOT(onSocketReadyRead()));

    if (ui->use_angel_beats->isChecked())
    {
        m_heartbeat->start();
    }

    ui->btnSend->setEnabled(true);

    emit sig_Communicationstate(true);

}

//客户端接入时 文本框的显示
void TCPServeronlyone::onClientConnected()
{
    clientnumb++;

   show_localtext(tr("**客户端接入  目前数量: ") + QString::number(clientnumb));
    for (auto& i : tcpSockets)
    {
       show_localtext(tr("**客户端地址:") +
            i->peerAddress().toString());
       show_localtext(tr("**客户端端口:") +
            QString::number(i->peerPort()));
    }

    connectionisok = true;
}

//客户端断开连接时 文本框的显示以及 断开连接
void TCPServeronlyone::onClientDisconnected()
{
    nowprice++;
    clientnumb--;

   show_localtext(tr("客户端断开连接  目前数量： ") + QString::number(clientnumb));

    if (nowprice == tcpSockets.size())
    {
        for (auto& i : tcpSockets)
        {
            i->deleteLater();
        }
        tcpSockets.clear();
        ui->btnSend->setEnabled(false);
        connectionisok = false;
        sig_Communicationstate(false);

        nowprice = 0;
    }

    //    deleteLater();//QObject::deleteLater();
}

//发送信息  发送一行字符串，以换行符结束
void TCPServeronlyone::on_btnSend_clicked()
{


    if (connectionisok)
    {
        QString  msg = ui->editMsg->text();
        show_localtext(tr("[发送] ") + msg);
        ui->editMsg->clear();
        ui->editMsg->setFocus();
        QByteArray  str = msg.toUtf8();
        str.append('\n');//添加一个换行符
        for (auto& i : tcpSockets)
        {
            i->write(str);
        }

    }
    else
    {
       show_localtext(tr("发送信息失败,通讯未连接"));
    }

}


//收消息
void TCPServeronlyone::onSocketReadyRead()
{//读取缓冲区行文本
//    QStringList   lines;
 //  while (tcpSocket->canReadLine()) {show_localtext("[in] " + tcpSocket->readLine()); }

        for (auto& i : tcpSockets)
        {
            char buffer[1024] = { 0 };


            i->read(buffer, 1024);

           if (strlen(buffer) > 0)
           {
               //  undefined
               QString showNsg = QString::fromLocal8Bit(buffer);
              show_localtext(tr("[接收] ") + showNsg);

               if (ui->use_angel_beats && showNsg == ui->recive_angel_beats->text())
               {
                   if (ui->use_angel_beats->isChecked())
                   {
                       m_heartbeat->stop();
                       m_heartbeat->start();
                       SendData(ui->send_angel_beats->text());
                   }              
               }
               emit sig_ReceivedData(showNsg);
           }
        }



}

void TCPServeronlyone::on_actStart_triggered()
{
 

    //开始监听
    QString     IP = ui->lineIP->text();//IP地址
    quint16     port=ui->spinPort->value();//端口
    QHostAddress    addr(IP);
    tcpServer->listen(addr,port);   //     绑定IP地址和端口号 

    //显示文本
//    tcpServer->listen(QHostAddress::LocalHost,port);
   show_localtext(tr("**开始监听..."));
   show_localtext(tr("**服务器 IP: ")
                       +tcpServer->serverAddress().toString());
   show_localtext(tr("**服务器 端口: ")
                       +QString::number(tcpServer->serverPort()));

    ui->actStart->setEnabled(false);
    ui->actStop->setEnabled(true);

}

void TCPServeronlyone::on_actStop_triggered()
{

    //停止监听
    if (tcpServer->isListening()) //tcpServer正在监听
    {
        tcpServer->close();//停止监听
    }

    ui->actStart->setEnabled(true);
    ui->actStop->setEnabled(false);


    for (auto& i : tcpSockets)
    {
        i->close();
        i->deleteLater();
    }
    tcpSockets.clear();
}

//清理文本框
void TCPServeronlyone::on_actClear_triggered()
{
    ui->plainTextEdit->clear();
}

//void TCPServeronlyone::on_actHostInfo_triggered()
//{//获取本机地址
//    QString hostName=QHostInfo::localHostName();//本地主机名
//   show_localtext(tr("本地名: ")+hostName+"\n");
//    QHostInfo   hostInfo=QHostInfo::fromName(hostName);
//
//    QList<QHostAddress> addList=hostInfo.addresses();//
//    if (!addList.isEmpty())
//    for (int i=0;i<addList.count();i++)
//    {
//        QHostAddress aHost=addList.at(i);
//        if (QAbstractSocket::IPv4Protocol==aHost.protocol())
//        {
//            QString IP=aHost.toString();
//           show_localtext(tr("本地IP地址: ")+aHost.toString());
//            if (ui->comboIP->findText(IP)<0)
//                ui->comboIP->addItem(IP);
//        }
//    }
//
//}

void TCPServeronlyone::on_use_angelbeats(int state)
{
    if (state == Qt::Checked && clientnumb != 0)
    {
        m_heartbeat->start();
    }
    else if (state == Qt::Unchecked)
    {
        m_heartbeat->stop();
    }

}

void TCPServeronlyone::on_angelbeats()
{
    for (auto& i : tcpSockets)
    {
        i->deleteLater();
    }
    tcpSockets.clear();
    ui->btnSend->setEnabled(false);
    connectionisok = false;
    sig_Communicationstate(false);


    m_heartbeat->stop();
    nowprice = 0;
    qDebug() << "断开连接";

}

void TCPServeronlyone::on_angelbeats_timeer_change(int)
{
    m_heartbeat->setInterval(ui->beat_timer->value() * 1000);
}

