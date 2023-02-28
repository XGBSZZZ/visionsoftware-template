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

	virtual void send_data(const QString&);  //������Ϣ

	virtual void Start() { on_use_bind_clicked(); }
private:
	QUdpSocket* UdpServer = new QUdpSocket;
	QTimer* m_heartbeat = new QTimer;

private:
	virtual void show_localtext(const QString&);

private:
	Ui::UDPserver ui;

private slots:
	virtual void receive_data();   //������Ϣ

private slots:
	virtual void on_clear_text_clicked();  //����ı���
	virtual void on_use_bind_clicked();  //��
	virtual void on_use_send_message_clicked();  //������Ϣ��ť
	virtual void on_join_receive_team_clicked();  //�����鲥��ť
	virtual void on_exit_receive_team_clicked();  //�˳��鲥��ť
	virtual void on_use_angel_beats_clicked(bool);  //�Ƿ�������ʹ������
	virtual void on_angel_beats_timer_change(int);  //��ʹ������Ƶ�ʸı�
	virtual void on_angelbeats();

signals:
	void  sig_ReceivedData(const QString&);
	void  sig_Communicationstate(const bool&);
};
