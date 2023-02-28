#pragma once

#include <QWidget>
#include "ui_CommunicationAdmini.h"
#include "qpushbutton.h"
#include "CommunicationBtn.h"
#include "QtWidgetsSaveLoad.h"

class CommunicationAdmini : public QWidget
{
	Q_OBJECT

public:
	explicit CommunicationAdmini(QWidget *parent = Q_NULLPTR);
	~CommunicationAdmini();

	void loaddata();

	void Send_Data(const QString& name, const QString& data);
public:
	QVector<CommunicationBtn*> allcommunication;

private:
	Ui::CommunicationAdmini ui;
	QPushButton* lhzbutten;
	CommunicationBtn* btn;
	QtWidgetsSaveLoad* saveloadwidget; 
	
private:
	void savedata();
	void closeEvent(QCloseEvent* e);

signals:
	void sig_Add_Change(const QString& name, const QString& type);
	void sig_Delete_Change(const QString& name, const QString& type);
	void sig_Rceive_Data(const QString& name, const QString& data);
	void sig_Show_Text(const QString& text);
};
