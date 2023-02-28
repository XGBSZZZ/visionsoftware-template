#include "CommunicationBtn.h"

CommunicationBtn::CommunicationBtn(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//设置无边框
	this->setWindowFlags(Qt::FramelessWindowHint);

    connect(communicationadd, &CommunicationAdd::Add_Change, this, &CommunicationBtn::Add_Change);
    connect(communicationadd, &CommunicationAdd::GG_Change, this, &CommunicationBtn::GG_Change);
}

CommunicationBtn::~CommunicationBtn()
{

}

void CommunicationBtn::mouseReleaseEvent(QMouseEvent* event)
{
    // 如果是鼠标左键按下
    if (event->button() == Qt::LeftButton)
    {
      
    }
    // 如果是鼠标右键按下
    else if (event->button() == Qt::RightButton && communicationname != "unknow")
    {
        if (communicationname != "unknow" && communicationtype != "unknow")
        {
            delog();
        }
    }
}

void CommunicationBtn::delog()
{
    communicationadd->hide();
    communicationadd->show();
}

void CommunicationBtn::on_Add_clicked()
{
    if (communicationname == "unknow" && communicationtype == "unknow")
    {
        delog();     
    }
    else
    {
        if (communicationtype == "TCPserver")
        {
            tcpserver->hide();
            tcpserver->show();
            tcpserver->setWindowTitle(communicationtype + "—" + communicationname);
        }

        else if (communicationtype == "TCPclient")
        {
            tcpclient->hide();
            tcpclient->show();
            tcpclient->setWindowTitle(communicationtype + "—" + communicationname);
        }
        else if (communicationtype == "UDP")
        {
            udp->hide();
            udp->show();
            udp->setWindowTitle(communicationtype + "—" + communicationname);
        }

    }
}

void CommunicationBtn::GG_Change()
{
    if (communicationname != "unknow" && communicationtype != "unknow")
    {
        emit sig_Delete_Change(communicationname, communicationtype);
    }


    if (NULL != tcpserver )
    {
        delete tcpserver;
        tcpserver = NULL;
    }

    if (NULL != tcpclient )
    {
        delete tcpclient;
        tcpclient = NULL;
    }

    if (NULL != udp)
    {
        delete udp;
        udp = NULL;
    }

    communicationname = "unknow";
    communicationtype = "unknow";
    ui.Add->setText("");
    ui.Add->setIcon(QIcon("myfile/images/ADD.png"));
    ui.Add->setStyleSheet("background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);");

    communicationadd->close();
}

void CommunicationBtn::Add_Change(const QString& name, const int& type)
{
     GG_Change();
     ui.Add->setStyleSheet("border-image:url(myfile/images/gg.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);");
     communicationname = name;

     if (type == 0)
     {
         communicationtype = "TCPserver";

         tcpserver = new TCPServeronlyone;

         connect(tcpserver, &TCPServeronlyone::sig_ReceivedData, [=](const QString& data) {
             emit sig_ReceivedData(communicationname, data);});
         connect(tcpserver, &TCPServeronlyone::sig_Communicationstate, [=](const bool& state) {
             emit sig_Communicationstate(communicationname, state);
             if (state)
             {
                 ui.Add->setStyleSheet("border-image:url(myfile/images/bind.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);");
             }
             else
             {
                 ui.Add->setStyleSheet("border-image:url(myfile/images/gg.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);");
             }
             }); 
     }

     else if (type == 1)
     {
         communicationtype = "TCPclient";
         tcpclient = new TCPclient;
         connect(tcpclient, &TCPclient::sig_ReceivedData, [=](const QString& data) {
             emit sig_ReceivedData(communicationname, data); });
         connect(tcpclient, &TCPclient::sig_Communicationstate, [=](const bool& state) {
             emit sig_Communicationstate(communicationname, state); 
             if (state)
             {
                 ui.Add->setStyleSheet("border-image:url(myfile/images/bind.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);");
             }
             else
             {
                 ui.Add->setStyleSheet("border-image:url(myfile/images/gg.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);");
             }
             });
     }

     else if (type == 2)
     {
         communicationtype = "UDP";

         udp = new UDPserver;
         connect(udp, &UDPserver::sig_ReceivedData, [=](const QString& data) {
             emit sig_ReceivedData(communicationname, data); });
         connect(udp, &UDPserver::sig_Communicationstate, [=](const bool& state) {
             emit sig_Communicationstate(communicationname, state); 
             if (state)
             {
                 ui.Add->setStyleSheet("border-image:url(myfile/images/bind.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);");
             }
             else
             {
                 ui.Add->setStyleSheet("border-image:url(myfile/images/gg.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);");
             }
             });
     }

     ui.Add->setText(communicationtype + "\r\n" + name);
     ui.Add->setIcon(QIcon(""));

     emit sig_Add_Change(communicationname, communicationtype);
}

void CommunicationBtn::Start()
{
    if (communicationtype == "TCPserver")
    {
        tcpserver->Start();
    }
    else if (communicationtype == "TCPclient")
    {
        tcpclient->Start();
    }
    else if (communicationtype == "UDP")
    {
        udp->Start();
    }
}
