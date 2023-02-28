#pragma once
#include "../CameraInterface/camerainterface.h"
#include <mutex>
#include "Windows.h"
#include "CameraApi.h"

#ifndef _WIN64
#pragma comment (lib,"MVCAMSDK")
#else
#pragma comment (lib,"MVCAMSDK_X64")
#endif // _WIN32



enum CameraTriggerType
{
	AUTOTRIGGER = 0,
	SOFTTRIGGER = 1,
	HARDTRIGGER = 2
};

class MindVisionCameraInterface :public CameraInterface
{

public:
	MindVisionCameraInterface(CameraTriggerType = AUTOTRIGGER);
	virtual ~MindVisionCameraInterface() override;
	virtual bool is_open();
	virtual void open_camera(const int&);
	virtual cv::Mat get_cvmat(const int&,const bool& flag = false);
	virtual std::string getSerialName();
	void openWithSn(const std::string&,const int& = 0);
	void setRgistedStr(const std::string&);
	void showCameraSetting();
	void setCameraTriggerType(const CameraTriggerType&);
	std::map<std::string, int> getCameraList();
private:
	std::string serialName;
	bool is_opened;
	int camera_handle;
	tSdkCameraCapbility about_camera;
	unsigned char* out_raw_data_ptr;
	unsigned char* out_data_ptr;
	tSdkFrameHead data_head;
	std::mutex mutex;
	std::string reg_str;
	CameraTriggerType type;
	char title[1024];
};