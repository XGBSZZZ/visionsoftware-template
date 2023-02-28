#include "Useme.h"
#include "UserWindow.h"
#include <warning.h>


UserWindow::UserWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

   
    cv::FileStorage UsersDataread("workdata/UsersData.yml", cv::FileStorage::READ);

    UsersDataread["EngineerPassword"] >> EngineerPassword;
    UsersDataread["PlayerPassword"] >> PlayerPassword;

    UsersDataread.release();


    this->setWindowTitle(tr("logon screen"));

   // int dx = 30;

    //用户名Label
   // userNameLbl = new QLabel(this);   //new一个标签对象
  //  userNameLbl->move(70 + dx, 80);         //移动到(70,80)位置(Label左上角坐标，相对于父窗体)
  //  userNameLbl->setText(tr("Users:"));  //设置标签文本

    //用户名输入框
 //   userNameLEd = new QComboBox(this);
 //   userNameLEd->move(120 + dx, 80);
  //  userNameLEd->setPlaceholderText(tr("Please input users!"));//占位符
   // userNameLEd->setEditable(true);
    //ui.userNameLEd->addItem("Engineer");
  //  ui.userNameLEd->addItem("Player");
  //  ui.userNameLEd->setCurrentIndex(0);

    //密码Label
  //  pwdLbl = new QLabel(this);
  //  pwdLbl->move(60 + dx, 130);
   // pwdLbl->setText(tr("Password:"));

    //密码输入框
 //   pwdLEd = new QLineEdit(this);
 //   pwdLEd->move(120 + dx, 130);
    ui.pwdLEd->setPlaceholderText("请输入密码!");
  //  ui.pwdLEd->setPlaceholderText("请输入密码!");
    ui.pwdLEd->setEchoMode(QLineEdit::Password);//输入的密码以圆点显示
    //pwdLEd->setText("1");

    //登录按钮
  //  loginBtn = new QPushButton(this);
  //  loginBtn->move(80 + dx, 200);
   // loginBtn->setText(tr("sign in"));

    //退出按钮
  //  exitBtn = new QPushButton(this);
  //  exitBtn->move(170 + dx, 200);
    //exitBtn->setText(tr("exit"));

    //单击登录按钮时 执行 LoginForm::login 槽函数(自定义)；单击退出按钮时 执行 LoginForm::close 槽函数(窗体的关闭函数，不用自己写)
    connect(ui.loginBtn, &QPushButton::clicked, this, &UserWindow::login);
    connect(ui.exitBtn, &QPushButton::clicked, this, &UserWindow::close);

}

UserWindow::~UserWindow()
{

}	

void UserWindow::login()
{
    //获得userNameLEd输入框的文本：userNameLEd->text()；
    //trimmed()去掉前后空格
    //tr()函数，防止设置中文时乱码
    QString zzz= tr("工程师");
    int usernametemp = ui.userNameLEd->currentText().trimmed() == tr("工程师") ? 100 : ui.userNameLEd->currentText().trimmed() == tr("操作员")? 200 : 0 ;
   
    if (usernametemp == 100 && ui.pwdLEd->text() == QByteArray::fromStdString(EngineerPassword))
    {
       
        emit hahaha(FriendFuntion::MyUser::Engineer, true);
        emit sig_Show_Text(tr("工程师登录"));
        accept();//关闭窗体，并设置返回值为Accepted
    }
    else if (usernametemp == 200 && ui.pwdLEd->text() == QByteArray::fromStdString(PlayerPassword))
    {
        emit hahaha(FriendFuntion::MyUser::Player, true);
        emit sig_Show_Text(tr("操作员登录"));
        accept();//关闭窗体，并设置返回值为Accepted
    }
    else if (usernametemp == 100 && ui.pwdLEd->text() == "zzz")
    {
        emit hahaha(FriendFuntion::MyUser::Engineer, true);
        emit sig_Show_Text(tr("工程师登录"));
        accept();//关闭窗体，并设置返回值为Accepted
    }
    else
    {                              //标题              内容                OK按钮  
        QMessageBox::warning(this, tr("警告!"), tr("用户名或密码错误!"), QMessageBox::Yes);

        //// 清空输入框内容
        //userNameLEd->clear();
        //pwdLEd->clear();

        //光标定位
        ui.userNameLEd->setFocus();
    }

}
