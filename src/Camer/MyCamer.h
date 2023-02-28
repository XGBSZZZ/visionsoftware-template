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
	FriendFuntion::MyCameranames camname = FriendFuntion::MyCameranames::Null;  //���ù��ң���ȥ���������ʼ����
	bool camerrun = false;
	bool alocopyok = true;
	std::vector<cv::Mat> demo_image;
	std::vector<cv::Mat> buffer_image;
	int demoimagenumb = 0;
	int buffernumb = 100;
	int nowbuffer = 0;
private:
//������ʵ�������
//�ٸ�����   �ҵ����Ϊ��  Mycanmer1
// 
//���������ָ��  �ж��ٸ�����Ͷ������ָ��   ����������Ĳ�ʹ�õ����  �ǵ�ɾ����ע�͵���ֹ�ڴ��˷�
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
