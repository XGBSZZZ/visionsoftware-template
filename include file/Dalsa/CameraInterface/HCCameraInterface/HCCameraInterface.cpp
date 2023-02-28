#include "HCCameraInterface.h"

#include <functional>

HCCameraInterface::HCCameraInterface() :
	camera_handle(nullptr),
	opened(false)
{
	 MV_CC_EnumDevices(MV_GIGE_DEVICE, &device_list);
}

HCCameraInterface::~HCCameraInterface()
{
	if (camera_handle != nullptr)
	{
		MV_CC_CloseDevice(camera_handle);
		MV_CC_DestroyHandle(camera_handle);
	}
}

bool HCCameraInterface::is_open()
{
	return opened;
}

void HCCameraInterface::open_camera(const int& idx)
{
	if (MV_CC_CreateHandle(&camera_handle, device_list.pDeviceInfo[idx]) != MV_OK)
	{
		camera_handle = nullptr;
		throw CameraError("相机无法打开。");
		return;
	}

	if (MV_CC_OpenDevice(camera_handle) != MV_OK)
	{
		MV_CC_DestroyHandle(camera_handle);
		camera_handle = nullptr;
		throw CameraError("相机无法打开。");
	}
	int rtn;
	rtn = MV_CC_SetEnumValue(camera_handle, "TriggerMode", false);

	rtn = MV_CC_StartGrabbing(camera_handle);

	rtn = MV_CC_GetIntValue(camera_handle, "PayloadSize", &frame_param);

	opened = true;
}

cv::Mat HCCameraInterface::get_cvmat(const int & type, bool flag)
{
	if (!opened)
		throw CameraError("相机未打开，无法获取图像。");

	std::vector<uchar> frame_data(frame_param.nCurValue);

	MV_FRAME_OUT_INFO_EX info;

	int nTestFrameSize = 0;
	while (1)
	{
		int nRet;
		if (nTestFrameSize > 99)
		{
			throw CameraError("获取图像超时。");
		}
		nRet = MV_CC_GetOneFrameTimeout(camera_handle, frame_data.data(), frame_data.size(), &info, 1000);
		if (MV_OK == nRet)
		{
			break;
		}
		else
		{
			nTestFrameSize++;
		}
	}
	if (info.enPixelType == MvGvspPixelType::PixelType_Gvsp_Mono8)
	{
		cv::Mat out(info.nHeight, info.nWidth, CV_8UC1, frame_data.data());
		return out.clone();
	}
	else
	{
		cv::Mat out(info.nHeight, info.nWidth, CV_8UC3, frame_data.data());
		if (type == CV_8UC1)
		{
			cv::Mat out_c1;
			cv::cvtColor(out, out_c1, cv::COLOR_RGB2GRAY);
			return out_c1.clone();
		}
		else
		{
			return out.clone();
		}
	}
	
}

std::vector<std::string> HCCameraInterface::getCameraList()
{
	return std::vector<std::string>();
}
