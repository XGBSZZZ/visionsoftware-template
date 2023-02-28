#pragma once

#include <QWidget>
#include "ui_WorkParametricWindow.h"
#include "qlineedit.h"
#include "Useme.h"
#include "QtWidgetsSaveLoad.h"
#include "ProductsAdd.h"


class WorkParametricWindow : public QWidget
{
	Q_OBJECT

public:
	WorkParametricWindow(QString savepaths , QWidget *parent = Q_NULLPTR);
	~WorkParametricWindow();
	void otherinit();

private:
	friend class MainWindows;
	friend class MyWorkWindow;
	QString savepath = "unknow";
	//QVector<QLineEdit*> all_label;
	QVector<QLineEdit*> all_parametric;
	QVector<QLineEdit*> all_outputname;
	QVector<QCheckBox*> all_save;
	QVector<QCheckBox*> all_tcp;
	QVector<QCheckBox*> all_udp;
	std::shared_ptr<QtWidgetsSaveLoad> wedgetsaveload;
	ProductsAdd* productsadd = nullptr;
	QString tempname;
	int tcplabel;
	int udplabel;


private:
	void closeEvent(QCloseEvent* e);
	void save_Data();
	void load_Data();
private:
	Ui::WorkParametricWindow ui;

private slots :
    virtual void On_checkBox();
	virtual void on_aloparametric(const QString&);
	virtual void communication_chage(const QString&,const QString&);
	virtual void communication_delete(const QString&,const QString&);
	virtual void on_btn_alo_input_clicked();
	virtual void on_btn_alo_output_clicked();
	virtual void on_btn_addproducts_clicked();
	virtual void on_comboxproducts(int);

signals:
	void workparametric_state(const bool&,const bool&,const bool&,const bool&);
	void aloparametric_change();
	void show_text(const QString&);
	void sig_output();
	void sig_title();

};
