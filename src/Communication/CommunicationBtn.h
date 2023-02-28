#pragma once

#include <QWidget>
#include "ui_CommunicationBtn.h"
#include <QContextMenuEvent>
#include "Useme.h"
#include "qdebug.h"
#include "TCPserver.h"
#include "TCPclient.h"
#include "UDPserver.h"
#include "CommunicationAdd.h"
#include "FriendFuntion.h"
#include <QFile>


class CommunicationBtn : public QWidget
{
	Q_OBJECT

public:
	explicit CommunicationBtn(QWidget *parent = Q_NULLPTR);
	~CommunicationBtn();

	QString communicationname = "unknow";
	QString communicationtype = "unknow";

	void Add_Change(const QString&, const int&);
	void Start();

public:
	CommunicationAdd* communicationadd = new CommunicationAdd;
	TCPServeronlyone* tcpserver = NULL;
	TCPclient* tcpclient = NULL;
	UDPserver* udp = NULL;

	
private:
	void mouseReleaseEvent(QMouseEvent* event);
	void delog();

private slots:
	void on_Add_clicked();
	void GG_Change();

signals:
	void sig_Add_Change(const QString& name, const QString& type);
	void sig_Delete_Change(const QString& name, const QString& type);
	void sig_ReceivedData(const QString& name, const QString& data);
	void sig_Communicationstate(const QString& name , const bool&);

private:
	Ui::CommunicationBtn ui;
};
