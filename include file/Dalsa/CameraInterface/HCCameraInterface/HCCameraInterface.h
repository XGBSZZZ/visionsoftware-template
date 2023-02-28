#pragma once

#include "../CameraInterface/camerainterface.h"
#include "windows.h"
#include <QObject>
#include "MvCameraControl.h"
class HCCameraInterface :public CameraInterface
{
public:
	HCCameraInterface();

	~HCCameraInterface();
	// Í¨¹ý CameraInterface ¼Ì³Ð
	virtual bool is_open() override;

	virtual void open_camera(const int&) override;

	virtual cv::Mat get_cvmat(const int &, bool flag = false);
	std::vector<std::string> getCameraList();
private:
	MV_CC_DEVICE_INFO camera_info;
	void* camera_handle;
	bool opened;
	MV_CC_DEVICE_INFO_LIST device_list;
	MVCC_INTVALUE frame_param;
};
