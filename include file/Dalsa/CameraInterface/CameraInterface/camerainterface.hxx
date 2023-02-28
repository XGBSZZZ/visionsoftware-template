#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H

#include <exception>
#include <string>
#include <filesystem>
#include <map>
class CameraError : public std::exception
{
public:
	CameraError(const std::string& w) :msg(w) {};
	~CameraError() {};
	const char* what() const { return msg.data(); };
private:
	std::string msg;
};

enum CameraTriggerType
{
	SoftTrigger,
	HardTrigger,
	AutoTrigger
};
namespace cv
{
	class Mat;
}
class QImage;
class CameraInterface
{
public:
	CameraInterface() : type(CameraTriggerType::AutoTrigger) {};
	virtual ~CameraInterface() {};
	virtual bool isOpen() = 0;
	virtual void openCamera(const int&) = 0;
	virtual void openWithSn(const std::string&) = 0;
	virtual void setCameraTriggerType(const CameraTriggerType& t)
	{
		type = t;
	}
	CameraTriggerType getCameraTriggerType()
	{
		return type;
	}
	virtual cv::Mat getCvMat(const int&, const bool& flag = false) = 0;
	virtual QImage getQImage(const int&, const bool& flag = false) = 0;
	virtual std::map<std::string, int> getCameraList() = 0;
	virtual std::string getSerialName() = 0;
    virtual void showCameraSettingDialog() = 0;
private:
	CameraTriggerType type;
};


#endif // CAMERAINTERFACE_H
