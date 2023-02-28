#pragma once

#include <QWidget>
#include "ui_MainParametricWindow.h"
#include "Useme.h"
#include "qcheckbox.h"
#include "QtWidgetsSaveLoad.h"

class MainParametricWindow : public QWidget
{
	Q_OBJECT

public:
	MainParametricWindow(unsigned int worknumb , QWidget *parent = Q_NULLPTR);
	~MainParametricWindow();
	QVector<QCheckBox*> allqcheckbox;
	QLineEdit* timeer;
	friend class MainWindows;
private:
	QLabel* qlabel_localmod;
	QtWidgetsSaveLoad* wedgetsaveload;
	void closeEvent(QCloseEvent* e);
public:
	Ui::MainParametricWindow ui;
private slots:    
	virtual void on_workdata_savapath_change_clicked();
	virtual void mainparametric_change(int state);
signals:
	void textchange();
	void savapathchange(const QString&);
	void day_night_change();
};
