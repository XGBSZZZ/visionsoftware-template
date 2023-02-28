#pragma once

#include "Useme.h"
#include "ui_MainWindows.h"
#include <cstdlib>

//QT的申明
#include <QtWidgets/QMainWindow>
#include<QGraphicsScene>  
#include<QGraphicsView>                   //graphicsview类
#include <qfiledialog.h>                //getopenfilename 类申明
#include <qlabel.h>                     //label类
#include <QDateTime.h>
#include <QTranslator>

//opencv申明
#include <opencv2/opencv.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\types_c.h> 
#include <opencv2\highgui\highgui.hpp>  

//引用的本地头文件
#include "Useopencv.h"
#include "UserWindow.h"
#include "FriendFuntion.h"
#include "MyWorkWindow.h"
#include "UserAdmini.h"
#include "workThread.h"
#include "QLoadingWidget.h"
#include "MainParametricWindow.h"
#include "qtimer.h"
#include "CommunicationAdmini.h"

//                            _ooOoo_
//                           o8888888o
//                           88" . "88
//                           (| -_- |)
//                           O\  =  /O
//                        ____/`---'\____
//                      .'  \\|     |//  `.
//                     /  \\|||  :  |||//  \
//                    /  _||||| -:- |||||-  \
//                    |   | \\\  -  /// |   |
//                    | \_|  ''\---/''  |   |
//                    \  .-\__  `-`  ___/-. /
//                  ___`. .'  /--.--\  `. . __
//               ."" '<  `.___\_<|>_/___.'  >'"".
//              | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//              \  \ `-.   \_ __\ /__ _/   .-` /  /
//         ======`-.____`-.___\_____/___.-`____.-'======
//                            `=---='
//        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//                      佛祖保佑       永无BUG
//
//
//      ┏┛ ┻━━━━━┛ ┻┓
//      ┃　　　　　　 ┃
//      ┃　　　━　　　┃
//      ┃　┳┛　  ┗┳　┃
//      ┃　　　　　　 ┃
//      ┃　　　┻　　　┃
//      ┃　　　　　　 ┃
//      ┗━┓　　　┏━━━┛
//        ┃　　　┃   神兽保佑
//        ┃　　　┃   软件无BUG！
//        ┃　　　┗━━━━━━━━━┓
//        ┃　　　　　　　    ┣┓
//        ┃　　　　         ┏┛
//        ┗━┓ ┓ ┏━━━┳ ┓ ┏━┛
//          ┃ ┫ ┫   ┃ ┫ ┫
//          ┗━┻━┛   ┗━┻━┛


//追魂夺命流水线 ， 暗无天日鬼车间， 生死轮回两班倒，提桶跑路在明天。
//提最红的桶,进最黑的厂。
//初遇老哥进黑厂,再见已是提桶人。
//这软件Bug可真多
//GRIS
//跟你们一起做项目像坐牢
//这个年头请假都这么难吗  回来记得请假
//心若简单 世界皆是童话


struct  WorkerInitData {
    QString workname ;   //工位名字
    FriendFuntion::MyCameranames camername; //用什么取图  本地？相机？  哪款相机？
    FriendFuntion::MyAlgorithms algname;  //用什么算法
    std::string souceimage;  //放一张该工位的原图
    FriendFuntion::MyCalib mycalib;
};


class MainWindows : public QMainWindow
{
       Q_OBJECT

           Q_CLASSINFO("version", "1.0.0")
           Q_CLASSINFO("developer", "tang")

           Q_CLASSINFO("version", "1.0.1")
           Q_CLASSINFO("developer", "")//写上你的姓氏

public:
    explicit  MainWindows(QWidget *parent = Q_NULLPTR);
    ~MainWindows(); //析构函数

    Ui::MainWindows ui;

protected:


private:
    QLoadingWidget* qloadingwidget = new QLoadingWidget;
   // LodingThread* lodingthread = new LodingThread;
    QVector<MyWorkWindow*> myworkwindow;
    UserWindow* userwindow = new UserWindow;
    UserAdmini* useradmini;
    MainParametricWindow* mainparametricwindow;
    QTimer* testqtimer;
    QFile Daybookfile;
    saveTextThread* savewarningtext = new saveTextThread("workdata/WarningMessage/");
    CommunicationAdmini* communicationadmini = new CommunicationAdmini;
    //QTranslator* translator = new QTranslator;
private:
  
    const  WorkerInitData work1{
          "i am work1",
          FriendFuntion::MyCameranames::Null,
          FriendFuntion::MyAlgorithms::Defect ,
          "myfile/mydrawing.png",
          FriendFuntion::MyCalib::Line_Scan_Calib,
    };

  const  WorkerInitData work2{
       "我是工位2",
       FriendFuntion::MyCameranames::Null,
       FriendFuntion::MyAlgorithms::Defect,
       "myfile/mydrawing.png",
        FriendFuntion::MyCalib::Line_Scan_Calib,
  };

  const  WorkerInitData work3{
       "你说起那么长的名字有什么卵用吗",
       FriendFuntion::MyCameranames::Null,
       FriendFuntion::MyAlgorithms::Demo,
       "myfile/mydrawing.png",
        FriendFuntion::MyCalib::Line_Scan_Calib,
  };


    //将工位全部丢进去，当然你想用几个工位还是得用 worknumb 来设置 目前的Ui设计最好不超过3个
    const  std::vector<WorkerInitData> works = {work1,work2};

private:
    //懒狗专用 去除用户登录界面
    const bool quicklytest = true; 

    //原图の缓存    如果存的原图有问题就需要增加这个    需要更多的内存
    const int camerbuffer = 10;

    //每个工位开几个算法线程 越高越稳定但是这需要更好性能更多线程的CPU 如果线程不够了 请尝试增加该值
    const int alonumb = 20;

    //结果图の缓存  如果存的结果图有问题就需要增加这个  需要更多的内存  实际大小 = alonumb * savebuffer
    const int savebuffer = 1;  

    //显示线程の缓存
    const int displaybuffer = 30;


   //不需要设置
     FriendFuntion::MyUser UserName = FriendFuntion::MyUser::Engineer;
   // std::shared_ptr<TensorRunnerNativeInterface> runner_ptr= std::make_shared<TensorRunnerNativeInterface>("../b");

private:
    std::vector<QString> warningdatabuffer;  //不改
    unsigned int worknumb; //   要几个相机/工位  不改
    QMap<QString, QLabel*> allcommunicationstate;
private:
    virtual void Init_MyWorkWindow();   //MyWorkWindow的初始化
    virtual void Init_communication();   //通讯初始化
    virtual void Init_conn();   //信号绑定初始化
    virtual void Init_Ui();   //界面Ui的初始化
    virtual void Init_TestTimer();  //用于压力测试的定时器の初始化

private slots:
    virtual void show_Text(const QString&);  //在生产日志中显示文字
    virtual void show_Image(const int& , const cv::Mat&);  //在工位显示图片
    virtual void save_warning_message();  //保存警告信息 立即保存
    virtual void save_warning_message(const QString&);  //将保存信息加入缓存 当缓存达到某个值时保存 这里为立即保存 这是给自己用的
    virtual void reset_communicationstate();  //重置设置通讯状态的Ui
    virtual void reset_maxdatanumb(int);
private slots:
    virtual void set_saveworkdatapath(const std::string& );  //设置工位数据保存路径
    virtual void set_saveworkdatapath(const QString& );  //设置工位数据保存路径
    virtual void creat_workfile(const std::string& );  //创建工位需要的文件夹 
    virtual void creat_workfile(const QString& );  //创建工位需要的文件夹 

private slots:
    virtual void on_ReceivedData(const QString& ,const QString &);  //所有通讯的收到消息都会在这里处理
    virtual void on_Userok(const FriendFuntion::MyUser& , const bool&);  //用户权限设置
    virtual void on_PasssWordChange(const std::string&, const std::string&);  //用户密码更改
    virtual void on_testtimeout();  //压力测试の定时器触发
    virtual void on_mainParametric();  //主界面参数の更改
private slots:
    virtual void on_actionStart_triggered(bool);  //开始运行按钮
    virtual void on_actionCommunication_triggered();  //通讯按钮
    virtual void on_actionUserSign_triggered();  //用户登录按钮
    virtual void on_actionExitSign_triggered();  //用户退出按钮
    virtual void on_actionUseradmini_triggered();  //用户管理按钮
    virtual void on_actionTips_triggered();  //提示按钮
    virtual void on_actionSetting_triggered();  //主参数设置按钮
    virtual void on_actionAllUseDemo_triggered(bool);  //压力测试按钮
    virtual void on_actionLanguage_triggered();  //语言按钮
    virtual void on_production_clicked();  
    virtual void on_dayNoteBook_clicked();
    virtual void on_data_clicked();
    virtual void on_btnPruduction_clicked();
    virtual void on_btnCommunication_clicked();
    virtual void on_btnCammer_clicked();
};
