#pragma once

#include "Useme.h"

#include <QMainWindow>
#include "ui_MyWorkWindow.h"

#include "Useopencv.h"
#include "FriendFuntion.h"
#include "workThread.h"
#include "UseAlgorithm.h"
#include "MyCamer.h"
#include "Useopencv.h"
#include "ImageView.h"
#include "WorkParametricWindow.h"
#include "ProductionWindows.h"
#include "qtablewidget.h"

struct DataBuffer {
    bool isnull = true;
    QString savebuffer;
    QString tcpbuffer;
    QString udpbuffer;
};

class MyWorkWindow
  : public QMainWindow
{
	Q_OBJECT

public:
    explicit MyWorkWindow(QWidget *parent = Q_NULLPTR);
	~MyWorkWindow();

private:
	Ui::MyWorkWindow ui;

private: 
    friend class MainWindows;
    MyCamer* mycamer = new MyCamer;  //相机线程
    QVector<UseAlgorithm*> alopool;  //算法线程池
    QVector<displayimageThread*> displayimagethreadpool;  //显示线程池
    WorkParametricWindow* workparametricwindow;  //参数界面
    ProductionWindows* productionwindows;  //生产管理界面
    saveTextThread* savealoerrcodetext;  //保存算法的结果
    QLabel* camerstate;  //相机状态显示
    QTableWidget* datadisplay;  //算法数据显示

private:
    int nowdisplaylabel = 0;
    int worklabel;//复读
    int databuffernumb = 100;//数据缓存总数
    int nowsendresult = 0;//发送结果数据索引
    std::vector<DataBuffer> databuffer;//数据缓存容器
    int alonumb;  //开几个算法线程 由主界面控制
    bool usealolabel = true;  //是否使用算法,不用管我，到时候会由一个按钮控制我
    bool usecontinulabel = false;  //是否连续运行 看上面
    QString imageisok = "UNKNOW";  //+1
    int nowalonumb = 0;  //+1
    cv::Mat image_view;  //+1
    std::vector<QString> ErrorcodeMessageBuffer;  //+1
    std::vector<QString> ProductionMessageBuffer;  //+1
    std::vector<QString> outputtext;
    std::vector<bool> outputsave;
    std::vector<bool> outputtcp;
    std::vector<bool> outputudp;
private:
    QString qsaveworkdatapath;
    std::string saveworkdatapath;
    int ngprice;
    int nowngprice = 0;
    int okprice;
    int nowokprice = 0;

private:
    bool m_isdaynight = false;
    int m_daytimehouse;
    int m_daytimemin;
    int m_nighttimehouse;
    int m_nighttimemin;

private:
    int now_datarow = 0;
    int max_datarow = 0;

private:
    bool m_needdelete = false;
    bool m_mustdelete = false;
    int m_maxday;
    int m_maxsize;
private:
    //FriendFuntion::MyUser UserName ;
    QString WorkName = tr("未初始化名字");  //同下
    std::string stdworkname;
    FriendFuntion::MyCalib Workcalib = FriendFuntion::MyCalib::Area_Scan_Calib;  //不用改
    FriendFuntion::MyAlgorithms AlgorithmName = FriendFuntion::MyAlgorithms::Demo;  //俺也一样

private:
    void Init();    //用于初始化
    virtual void contextMenuEvent(QContextMenuEvent* event) override; //用于图片查看
    void OtherInit(const int& buffernumb);  //用于主函数调用
    
private:
    virtual void show_data(const QString&);
    virtual void show_image(const cv::Mat&) ;  //写了存储和显示的线程の开启
    inline virtual void show_image_thread(const QImage& img);  //在界面显示图片
    inline virtual void show_text(const QString&) ;  //在文本框显示数据
    virtual void get_image(const int&, const cv::Mat&);  //回算法传数据给界面  可以改
    virtual void get_data(const Alooutputdata&);  //回传数据给界面
    virtual void butten_changge(const FriendFuntion::MyUser&, const bool&);  //用户权限改变
    inline virtual void save_warning_message(const QString&);  //保存警告信息
    inline virtual void save_production_data();  //保存生产数据  立即保存
    inline virtual void save_production_data(const QString&);  //保存生产数据  将生产数据加入缓存 当缓存到达一个量时保存
    inline virtual void Send_Data_TCP(const QString&);  //在绑定的TCP口发送消息
    inline virtual void Send_Data_UDP(const QString&);  //在绑定的UDP口发送消息
private:
    void mouseDoubleClickEvent(QMouseEvent* event) override;  //查看图片的鼠标响应事件

private slots:
    virtual void on_actionOpencamera_triggered();  //打开相机按钮
    virtual void on_actionSingle_triggered();  //运行采图按钮
    virtual void on_actioncontinuity_triggered(bool);  //深渊泰坦 —— 被遗弃、被遗忘  被废弃的按钮
    virtual void on_actionCalib_triggered();  //相机标定按钮
    virtual void on_actionUseDemo_triggered();  //运行例程按钮
    virtual void on_actionuseAlo_triggered(bool);  //使用算法按钮
    virtual void on_actionParameter_triggered();  //参数按钮

private slots:
    virtual void on_ImageView(const bool&);
    virtual void on_SaveImage(const bool&);
    virtual void on_savaimage(const cv::Mat&, const std::string&, const bool&);
    virtual void on_workparametric_state(const bool&, const bool&, const bool&, const bool&);
    virtual void on_aloparametric_change();
signals:
    void showtext(const QString&); 
    void savewarningmessage(const QString&);
    void sig_SendData_TCP(const QString& name, const QString& data);
    void sig_SendData_UDP(const QString& name, const QString& data);
};
