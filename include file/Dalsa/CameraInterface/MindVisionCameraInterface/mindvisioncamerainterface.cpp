#include "mindvisioncamerainterface.h"

MindVisionCameraInterface::MindVisionCameraInterface(CameraTriggerType tp) :
	camera_handle(-1),
	is_opened(false),
	out_raw_data_ptr(nullptr),
	out_data_ptr(nullptr),
	reg_str(""),
	type(tp),
	title("相机设置 - 微米立机器视觉平台")
{
}
MindVisionCameraInterface::~MindVisionCameraInterface()
{
	if (is_opened)
	{
		try
		{
			CameraStop(camera_handle);
			CameraUnInit(camera_handle);
		}
		catch (...)
		{
		}
	}
}

bool MindVisionCameraInterface::is_open()
{
	return is_opened;
}

void MindVisionCameraInterface::open_camera(const int& idx)
{
	int                     iCameraCounts = 4;
	int                     iStatus = -1;
	tSdkCameraDevInfo       tCameraEnumList[4];

	//sdk初始化  0 English 1中文
	CameraSdkInit(1);

	//枚举设备，并建立设备列表
	CameraEnumerateDevice(tCameraEnumList, &iCameraCounts);

	//没有连接设备
	if (iCameraCounts == 0) {
		throw CameraError(u8"没有连接的相机。");
	}

	//相机初始化。初始化成功后，才能调用任何其他相机相关的操作接口
	iStatus = CameraInit(&tCameraEnumList[idx], -1, -1, &camera_handle);

	//初始化失败
	if (iStatus != CAMERA_STATUS_SUCCESS) {
		throw CameraError(u8"初始化失败。");

	}
	//获得相机的特性描述结构体。该结构体中包含了相机可设置的各种参数的范围信息。决定了相关函数的参数
	CameraGetCapability(camera_handle, &about_camera);

	//g_pRgbBuffer = (unsigned char*)malloc(g_tCapability.sResolutionRange.iHeightMax*g_tCapability.sResolutionRange.iWidthMax * 3);
	//g_readBuf = (unsigned char*)malloc(g_tCapability.sResolutionRange.iHeightMax*g_tCapability.sResolutionRange.iWidthMax * 3);
	CameraSdkStatus status = CameraSetTriggerMode(camera_handle, int(type));
	/*让SDK进入工作模式，开始接收来自相机发送的图像
	数据。如果当前相机是触发模式，则需要接收到
	触发帧以后才会更新图像。    */
	CameraPlay(camera_handle);

	out_raw_data_ptr = new unsigned char[about_camera.sResolutionRange.iHeightMax * about_camera.sResolutionRange.iWidthMax * 3];
	out_data_ptr = new unsigned char[about_camera.sResolutionRange.iHeightMax * about_camera.sResolutionRange.iWidthMax * 3];

	/*
	设置图像处理的输出格式，彩色黑白都支持RGB24位
	*/
	CameraSetIspOutFormat(camera_handle, CAMERA_MEDIA_TYPE_MONO8);

	is_opened = true;
}

cv::Mat MindVisionCameraInterface::get_cvmat(const int &,const bool& flag)
{
	std::lock_guard<std::mutex> locker(mutex);
	if (type == CameraTriggerType::SOFTTRIGGER)
	{
		CameraSoftTrigger(camera_handle);
	}
	if (CameraGetImageBuffer(camera_handle, &data_head, &out_raw_data_ptr, 2000) == CAMERA_STATUS_SUCCESS)
	{
		CameraImageProcess(camera_handle,out_raw_data_ptr, out_data_ptr, &data_head);
		
		cv::Mat out(int(data_head.iHeight),int(data_head.iWidth), CV_8UC1,out_data_ptr);
		cv::Mat new_out;
		new_out = out.clone();
		cv::flip(out, new_out, 0);
		CameraReleaseImageBuffer(camera_handle, out_raw_data_ptr);
		//cv::rotate(out, out,);
		return new_out;
	}
	else
	{
		throw CameraError(u8"获取照片失败。");
	}
}

void MindVisionCameraInterface::openWithSn(const std::string & str,const int& idx)
{
	int                     iCameraCounts = 4;
	int                     iStatus = -1;
	tSdkCameraDevInfo       tCameraEnumList[4];

	//sdk初始化  0 English 1中文
	CameraSdkInit(1);

	//枚举设备，并建立设备列表
	CameraEnumerateDevice(tCameraEnumList, &iCameraCounts);

	//没有连接设备
	if (iCameraCounts == 0) {
		throw CameraError(u8"没有连接的相机。");
	}

	//相机初始化。初始化成功后，才能调用任何其他相机相关的操作接口
	if (str.empty())
	{
		throw CameraError(u8"初始化失败。");
	}
	tSdkCameraDevInfo* rinfo(nullptr);
	for (auto &info : tCameraEnumList)
	{
		bool flag(false);
		for (int i = 0; i < str.size(); i++)
		{
			if (i >= 32)
				return;
			if (info.acSn[i] != str[i])
			{
				flag = true;
				continue;
			}
		}
		if (flag)
			continue;
		rinfo = &info;
		break;
	}
	if (rinfo == nullptr)
	{
		throw CameraError(u8"初始化失败，未找到相机。");
	}
	iStatus = CameraInit(rinfo, PARAM_MODE_BY_SN,-1, &camera_handle);

	//初始化失败
	if (iStatus != CAMERA_STATUS_SUCCESS) {
		throw CameraError(u8"初始化失败：" + std::to_string(iStatus) + u8"。");
	}
	//获得相机的特性描述结构体。该结构体中包含了相机可设置的各种参数的范围信息。决定了相关函数的参数
	CameraGetCapability(camera_handle, &about_camera);
	CameraSdkStatus status = CameraSetTriggerMode(camera_handle,int(type));

	//g_pRgbBuffer = (unsigned char*)malloc(g_tCapability.sResolutionRange.iHeightMax*g_tCapability.sResolutionRange.iWidthMax * 3);
	//g_readBuf = (unsigned char*)malloc(g_tCapability.sResolutionRange.iHeightMax*g_tCapability.sResolutionRange.iWidthMax * 3);

	/*让SDK进入工作模式，开始接收来自相机发送的图像
	数据。如果当前相机是触发模式，则需要接收到
	触发帧以后才会更新图像。    */
	CameraPlay(camera_handle);

	out_raw_data_ptr = new unsigned char[about_camera.sResolutionRange.iHeightMax * about_camera.sResolutionRange.iWidthMax * 3];
	out_data_ptr = new unsigned char[about_camera.sResolutionRange.iHeightMax * about_camera.sResolutionRange.iWidthMax * 3];

	/*
	设置图像处理的输出格式，彩色黑白都支持RGB24位
	*/
	CameraSetIspOutFormat(camera_handle, CAMERA_MEDIA_TYPE_MONO8);

	is_opened = true;
	serialName = str;
}

void MindVisionCameraInterface::setRgistedStr(const std::string & str)
{
	reg_str = str;
}

void MindVisionCameraInterface::showCameraSetting()
{
	CameraCreateSettingPage(camera_handle, NULL,
		 title,NULL, NULL, 0);
	CameraShowSettingPage(camera_handle, TRUE);
}

void MindVisionCameraInterface::setCameraTriggerType(const CameraTriggerType & t)
{
	type = t;
	CameraSetTriggerMode(camera_handle, int(type));
}

std::map<std::string, int> MindVisionCameraInterface::getCameraList()
{
	return std::map<std::string, int>();
}

std::string MindVisionCameraInterface::getSerialName()
{
	return serialName;
}