#pragma once
#include "Camera/CameraInterface/camerainterface.h"
namespace BGAPI2
{
	class ImageProcessor;
	class DataStream;
	class System;
	class Interface;
	class Device;
	class Buffer;
}
class BaumerCameraInterface : public CameraInterface
{
public:
	BaumerCameraInterface();
	~BaumerCameraInterface();
	bool is_open();
	void open_camera(const int&);
	void openWithSn(const std::string&, const int& = 0);
	cv::Mat get_cvmat(const int&,const bool& flag = false);
	std::map<std::string, int> getCameraList();
	std::string getSerialName();
private:
private:
	std::string serial_name;
	BGAPI2::ImageProcessor * imgProcessor;
	BGAPI2::DataStream * pDataStream;
	BGAPI2::System *pSystem;
	BGAPI2::Interface *pInterface;
	BGAPI2::Device *pDevice;
	BGAPI2::Buffer * pBuffer;
	bool is_opened;
};

