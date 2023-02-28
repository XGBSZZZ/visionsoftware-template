#pragma once

#include <QWidget>
#include "ui_CommunicationAdd.h"

class CommunicationAdd : public QWidget
{
	Q_OBJECT

public:
	explicit CommunicationAdd(QWidget *parent = Q_NULLPTR);
	~CommunicationAdd();

private slots:
	void on_OK_clicked();
	void on_GG_clicked();

signals:
	void Add_Change(const QString& communicationName,const int& communicationType);
	void GG_Change();

private:
	Ui::CommunicationAdd ui;
};
