#pragma once

#include "Useme.h"


#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <qcombobox.h>
#include "ui_UserWindow.h"
#include "FriendFuntion.h"

class UserWindow : public QDialog
{
	Q_OBJECT

public:
	explicit UserWindow(QWidget *parent = Q_NULLPTR);
	~UserWindow();

public:
	


private:
	friend class MainWindows;
	Ui::UserWindow ui;
	//QComboBox* userNameLEd;      //用户名编辑行
	//QLineEdit* pwdLEd;           //密码编辑行
	//QLabel* userNameLbl;         //"用户名"标签
	//QLabel* pwdLbl;              //"密码"标签
	//QPushButton* loginBtn;       //登录按钮
	//QPushButton* exitBtn;        //退出按钮

	std::string EngineerPassword ;
	std::string PlayerPassword ;
private slots:
	void login();//点击登录按钮是执行的槽函数

signals:
	void hahaha( const FriendFuntion::MyUser& , const bool&);
	void sig_Show_Text(const QString&);
};
