#pragma once
#include <QThread>
#include "Useopencv.h"
#include "TensorRunnerNativeInterface.h"
#include "FriendFuntion.h"


static	TensorRunnerNativeInterface* Defect_bt = new TensorRunnerNativeInterface("profile/2943defect_bt.pb");
static	TensorRunnerNativeInterface* Defect_er = new TensorRunnerNativeInterface("profile/2943defect_er.pb");


class UseAlgorithm : public QThread
{
	Q_OBJECT

public:
	explicit UseAlgorithm(QObject *parent = 0);   //如果调用了深度学习要在里面进行初始化  即使用一张图片来跑 1 次
	~UseAlgorithm();
public:
	virtual void otherInit(const int& buffernumb);  //用于其他类调用的初始化变量函数
	virtual void run();
public:
private:
	virtual void useAlg();
	bool initlabel = true;
public:
	Useopencv* useopencv = new Useopencv;
	FriendFuntion::MyAlgorithms myalgrithms = FriendFuntion::MyAlgorithms::Demo;  //不用改
	Aloinputdata aloinputdata;  //算法所有的输入数据会存在这里
	Alooutputdata alooutputdata;  //算法所有的输出数据全部存这这里
	int nowsouce = 0;
	int nowresult = 0;
	int errlabel = 0;
	bool needsave = false;
	std::vector<cv::Mat> souce_buffer;
	std::vector<cv::Mat> result_buffer;
	bool saveoksouce;
	bool saveokresult;
	bool savengsouce;
	bool savengresult;

public:
	virtual void PbInit();
signals:
	void give_data(const Alooutputdata&); //回传数据给工位类   MyWorkWindow
	void show_text(const QString&);
	void sava_image(const cv::Mat&, const std::string& , const bool&);
    void save_warning_message(const QString&);
};
