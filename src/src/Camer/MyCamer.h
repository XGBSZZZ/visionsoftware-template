#pragma once

#include <QObject>
#include <opencv2/opencv.hpp>
#include "FriendFuntion.h"
#include <QThread>
#include "DalsaCameraInterface.h"


struct CamerParameters
{

};

class MyCamer : public QThread
{
	Q_OBJECT

public:
	MyCamer(QObject *parent = 0);
	~MyCamer();
	virtual void run();
public:
	virtual QString open_camer();
	virtual cv::Mat get_image();  
	virtual void set_parameters(const CamerParameters&);
	virtual void get_parameters(CamerParameters&);
	virtual bool isopen();
	virtual bool setcamerrun(const bool&);
	int Otherinit();
public:
private:
	friend class MainWindows;  
	friend class MyWorkWindow;
	FriendFuntion::MyCameranames camname = FriendFuntion::MyCameranames::Null;  //不用管我，请去主函数里初始化我
	bool camerrun = false;
	bool alocopyok = true;
	std::vector<cv::Mat> demo_image;
	std::vector<cv::Mat> buffer_image;
	int demoimagenumb = 0;
	int buffernumb = 100;
	int nowbuffer = 0;
private:
//在这里实例化相机
//举个例子   我的相机为类  Mycanmer1
// 
//定义相机的指针  有多少个相机就定义多少指针   如果有其他的不使用的相机  记得删除或注释掉防止内存浪费
//AreaScanCamer* areascancamer;
private:
private:
	DalsaCameraInterface* dalsecamer;
	std::string ccfPath_Dalse = "";
	std::string snNumber_Dalse = "12170717";
private slots:
signals:
	void give_image(const int&,const cv::Mat&); 
	void show_text(const QString&);
	void save_warning_message(const QString&);
	void sig_Run_State(const bool&);
};
