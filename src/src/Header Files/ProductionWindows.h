#pragma once

#include <QWidget>
#include "ui_ProductionWindows.h"
#include "QtWidgetsSaveLoad.h"
#include "FriendFuntion.h"
#include "workThread.h"

class ProductionWindows : public QWidget
{
	Q_OBJECT

public:
	explicit ProductionWindows(QWidget *parent = Q_NULLPTR);
	~ProductionWindows();

	saveProductionThread* saveproductionthread = new saveProductionThread;
private:
	Ui::ProductionWindows ui;
	friend class MyWorkWindow;
	QtWidgetsSaveLoad* qtwidgetsave;
public:
	QString savepath;
	virtual void load_data();
	virtual void save_data(const bool& mustday = false);
	int savepricenumb = 0;
public:
	bool m_isdaynight = false;
	int m_daytimehouse;
	int m_daytimemin;
	int m_nighttimehouse;
	int m_nighttimemin;
private:
	void is_ok();
	void is_ng();
private slots:
    virtual void on_pushButton_clicked();
};
