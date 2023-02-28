#pragma once

#include <QWidget>
#include "ui_TCPclient.h"

class TCPclient : public QWidget
{
	Q_OBJECT

public:
	explicit TCPclient(QWidget *parent = Q_NULLPTR);
	~TCPclient();

	void Send_Data(const QString&) {}
	void Start() {}
private:
	Ui::TCPclient ui;

signals:
	void  sig_ReceivedData(const QString&);
	void  sig_Communicationstate(const bool&);
};
