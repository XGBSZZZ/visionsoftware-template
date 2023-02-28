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
	explicit UseAlgorithm(QObject *parent = 0);   //������������ѧϰҪ��������г�ʼ��  ��ʹ��һ��ͼƬ���� 1 ��
	~UseAlgorithm();
public:
	virtual void otherInit(const int& buffernumb);  //������������õĳ�ʼ����������
	virtual void run();
public:
private:
	virtual void useAlg();
	bool initlabel = true;
public:
	Useopencv* useopencv = new Useopencv;
	FriendFuntion::MyAlgorithms myalgrithms = FriendFuntion::MyAlgorithms::Demo;  //���ø�
	Aloinputdata aloinputdata;  //�㷨���е��������ݻ��������
	Alooutputdata alooutputdata;  //�㷨���е��������ȫ����������
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
	void give_data(const Alooutputdata&); //�ش����ݸ���λ��   MyWorkWindow
	void show_text(const QString&);
	void sava_image(const cv::Mat&, const std::string& , const bool&);
    void save_warning_message(const QString&);
};
