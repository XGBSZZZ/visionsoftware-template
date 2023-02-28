
#pragma once
#include "CameraInterface/camerainterface.hxx"
#include "opencv2/opencv.hpp"
#include <QWidget>

class SapXferCallbackInfo;
class SapAcquisition;
class SapAcqDevice;
class SapFeature;
class SapBuffer;
class SapTransfer;
class SapGio;
class SapGioCallbackInfo;

class DalsaCameraInterface :
	 public QObject , public CameraInterface
{
	Q_OBJECT
public:
	DalsaCameraInterface(const std::string&,const int&);
	virtual ~DalsaCameraInterface();
	// Í¨¹ý CameraInterface ¼Ì³Ð
	virtual bool isOpen() override;
	virtual void openCamera(const int&) override;
	virtual void openWithSn(const std::string&) override;
	virtual cv::Mat getCvMat(const int& cn = 1, const bool& flag = false) override;
	virtual QImage getQImage(const int&, const bool& flag = false) override;
	virtual std::map<std::string, int> getCameraList() override;
	virtual std::string getSerialName() override;
	virtual void showCameraSettingDialog() override;
	virtual void changeParamFromMap(const std::map<std::string, std::tuple<std::string, std::string>>&) override;
	virtual void getParamFromCamera(std::map<std::string, std::tuple<std::string, std::string>>&) override;
	static void AcqCallback(SapXferCallbackInfo* pInfo);
	static void GioCallback(SapGioCallbackInfo *pInfo);
private:
	int pushimagelabel;
	int buffernumb;
	bool is_open;
	SapAcquisition* Acq;
	SapAcqDevice* AcqDevice;
	SapFeature* Feature;
	SapBuffer* Buffers;
	SapTransfer* Xfer;
	std::vector<SapGio*> gio;

	uint32_t  acqDeviceNumber;
	std::string acqServerName;
	std::string deviceServerName;
	std::string configFilename;
	std::string serial_name;
	int isNotSupported;
	int status; 
	int acquisitionCreated;
	int acqDeviceCreated;
	int nowimglabel;
	std::vector<cv::Mat> allimg;
	std::mutex mutex;
	bool rotated_flag;
	double exp_time_tmp;
	double gain_tmp;

signals:
    void sig_give_Buffer(const int&,const cv::Mat&);
};
