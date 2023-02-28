#include "Useme.h"
#include "UserAdmini.h"
#include <opencv2/core/persistence.hpp>

UserAdmini::UserAdmini(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose);
	ui.userNameLEd->addItem(("工程师"));
	ui.userNameLEd->addItem(("操作员"));
	ui.userNameLEd->setCurrentIndex(0);
}

UserAdmini::~UserAdmini()
{
	qDebug() << "UserAdmini out";
}

void UserAdmini::on_EX_clicked()
{
	this->close();
}

void UserAdmini::on_OK_clicked()
{
	cv::FileStorage UsersDatawriteread("workdata/UsersData.yml", cv::FileStorage::READ);
	std::string engineerpassword;
	std::string playerpassword;
	UsersDatawriteread["EngineerPassword"] >> engineerpassword;
	UsersDatawriteread["PlayerPassword"] >> playerpassword;

	UsersDatawriteread.release();
	//在release前不能覆盖写
	cv::FileStorage UsersDatawrite("workdata/UsersData.yml", cv::FileStorage::WRITE);

	if (ui.userNameLEd->currentText().trimmed() == tr("工程师"))
	{
		emit passswordchange("Engineer",ui.lineEdit->text().toStdString());
		UsersDatawrite << "EngineerPassword" << ui.lineEdit->text().toStdString();  //更改密码
		UsersDatawrite << "PlayerPassword" << playerpassword;   //因为是覆盖写所以要重写
	}

	else if (ui.userNameLEd->currentText().trimmed() == tr("操作员"))
	{
		emit passswordchange("Player", ui.lineEdit->text().toStdString());
		UsersDatawrite << "PlayerPassword" << ui.lineEdit->text().toStdString();
		UsersDatawrite << "EngineerPassword" << engineerpassword;
	}
	ui.lineEdit->setText("");//密码初始化

	UsersDatawrite.release();
}
