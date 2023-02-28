#pragma once
#include "Useme.h"
#include <QMainWindow>
#include "ui_UDPserver.h"
#include  "qudpsocket.h"
#include "qtimer.h"

class UDPserver : public QMainWindow
{
	Q_OBJECT

public:
	explicit UDPserver(QWidget *parent = Q_NULLPTR);
	~UDPserver();

	bool connectionisok = false;

	virtual void send_data(const QString&);  //发送消息

	virtual void Start() { on_use_bind_clicked(); }
private:
	QUdpSocket* UdpServer = new QUdpSocket;
	QTimer* m_heartbeat = new QTimer;

private:
	virtual void show_localtext(const QString&);

private:
	Ui::UDPserver ui;

private slots:
	virtual void receive_data();   //接收消息

private slots:
	virtual void on_clear_text_clicked();  //清除文本框
	virtual void on_use_bind_clicked();  //绑定
	virtual void on_use_send_message_clicked();  //发送消息按钮
	virtual void on_join_receive_team_clicked();  //加入组播按钮
	virtual void on_exit_receive_team_clicked();  //退出组播按钮
	virtual void on_use_angel_beats_clicked(bool);  //是否启用天使の心跳
	virtual void on_angel_beats_timer_change(int);  //天使の心跳频率改变
	virtual void on_angelbeats();

signals:
	void  sig_ReceivedData(const QString&);
	void  sig_Communicationstate(const bool&);
};
