#include "baslercamerainterface.hxx"
#include "pylon/PylonIncludes.h"
#include "opencv2/opencv.hpp"
#include <QDialog>
#include "ui_BaslerCameraSettingDialog.h"
class BaslerCameraSettingDialog : public QDialog
{
	Q_OBJECT

public:
	BaslerCameraSettingDialog(BaslerCameraInterface*, QWidget* parent = Q_NULLPTR);
	~BaslerCameraSettingDialog();

private:
	BaslerCameraInterface* camera_ptr;
	Ui::BaslerCameraSettingDialog ui;
};

BaslerCameraSettingDialog::BaslerCameraSettingDialog(BaslerCameraInterface* ptr, QWidget* parent)
	: QDialog(parent),
	camera_ptr(ptr)
{
	ui.setupUi(this);

}

BaslerCameraSettingDialog::~BaslerCameraSettingDialog()
{
}


using namespace Pylon;
BaslerCameraInterface::BaslerCameraInterface(const BaslerType& type) :
	camera_statu(false),
	camera_type(type)
{
	PylonInitialize();
}

BaslerCameraInterface::~BaslerCameraInterface()
{
	try
	{
		if (camera_statu)
		{
			camera->StopGrabbing();
			camera->Close();
			camera->DetachDevice();
		}

		PylonTerminate();
	}
	catch (...)
	{

	}
}

void BaslerCameraInterface::openCamera(const int& idx)
{
	CTlFactory& fac = CTlFactory::GetInstance();
	DeviceInfoList_t info_list;
	ITransportLayer* pTl(nullptr);

	switch (camera_type)
	{
	case BaslerCameraInterface::PYLON_GIGE:
		pTl = fac.CreateTl("BaslerGigE");
		break;
	case BaslerCameraInterface::PYLON_1394:
		pTl = fac.CreateTl("Basler1394");
		break;
	case BaslerCameraInterface::PYLON_USB:
		pTl = fac.CreateTl("BaslerUsb");
		break;
	case BaslerCameraInterface::PYLON_CLSER:
		pTl = fac.CreateTl("BaslerCLSer");
		break;
	}
	int re(0);
	while (true)
	{

		re++;
		pTl->EnumerateDevices(info_list);

		if (info_list.empty())
		{
			fac.ReleaseTl(pTl);
			throw CameraError("No Camera in this computer.");
			return;
		}
		if (info_list.size() <= idx)
		{
			fac.ReleaseTl(pTl);
			throw CameraError("Can't find the camera.");
			return;
		}
		try
		{
			camera = std::make_shared<Camera_t>(pTl->CreateDevice(info_list[idx]));
			camera->Open();
		}
		catch (GenICam::GenericException& e)
		{
			if (re > 3)
			{
				throw CameraError(std::string("Can't open camera \n Error:") + e.what());
			}
		}
	}
	camera_statu = true;
	camera->StartGrabbing(GrabStrategy_LatestImageOnly);
}


cv::Mat BaslerCameraInterface::getCvMat(const int& format, const bool& flag)
{
	std::lock_guard<std::mutex> locker(mutex);
	if (!camera_statu)
		return cv::Mat();
	int get_count(0);
	while (true)
	{
		get_count++;
		try
		{
			CGrabResultPtr res;
			cv::Mat out;
			camera->RetrieveResult(1000, res, TimeoutHandling_Return);

			uchar* data = static_cast<uchar*>(res->GetBuffer());
			if (format == CV_8UC3)
			{
				cv::Mat img(res->GetHeight(), res->GetWidth(), CV_8UC1);
				img.data = data;
				out = img.clone();
				cv::cvtColor(out, out, cv::COLOR_GRAY2BGR);
				return out;
			}
			else
			{
				cv::Mat img(res->GetHeight(), res->GetWidth(), CV_8UC1);
				img.data = data;
				out = img.clone();
				return out;
			}
		}
		catch (GenICam::GenericException& e)
		{
			if (get_count > 3)
			{
				throw CameraError(std::string("BaslerError:") + e.what());
			}
		}
	}

}

bool BaslerCameraInterface::isOpen()
{
	return camera_statu;
}

void BaslerCameraInterface::openWithSn(const std::string& sn)
{
	CTlFactory& fac = CTlFactory::GetInstance();
	DeviceInfoList_t info_list;
	ITransportLayer* pTl(nullptr);

	switch (camera_type)
	{
	case BaslerCameraInterface::PYLON_GIGE:
		pTl = fac.CreateTl("BaslerGigE");
		break;
	case BaslerCameraInterface::PYLON_1394:
		pTl = fac.CreateTl("Basler1394");
		break;
	case BaslerCameraInterface::PYLON_USB:
		pTl = fac.CreateTl("BaslerUsb");
		break;
	case BaslerCameraInterface::PYLON_CLSER:
		pTl = fac.CreateTl("BaslerCLSer");
		break;
	}
	pTl->EnumerateDevices(info_list);

	if (info_list.empty())
	{
		fac.ReleaseTl(pTl);
		throw CameraError("No Camera in this computer.");
		return;
	}
	try
	{
		bool flag(false);
		for (auto& info : info_list)
		{
			auto sn_str = info.GetSerialNumber();
			if (sn == sn_str.c_str())
			{
				camera = std::make_shared<Camera_t>(pTl->CreateDevice(info));
				camera->Open();
				flag = true;
				break;
			}
		}
		if (!flag)
			throw CameraError("Can't find the camera");
	}
	catch (GenICam::GenericException& e)
	{
		throw CameraError("Can't open Basler camera \n Error:" + std::string(e.what()));
	}
	serial_name = sn;
	camera_statu = true;
	camera->StartGrabbing(GrabStrategy_LatestImageOnly);
}

std::map<std::string, int> BaslerCameraInterface::getCameraList()
{
	return std::map<std::string, int>();
}

QImage BaslerCameraInterface::getQImage(const int& format, const bool& flag)
{
	std::lock_guard<std::mutex> locker(mutex);
	if (!camera_statu)
		return QImage();
	int get_count(0);
	while (true)
	{
		get_count++;
		try
		{
			CGrabResultPtr res;
			QImage out;
			camera->RetrieveResult(1000, res, TimeoutHandling_Return);

			uchar* data = static_cast<uchar*>(res->GetBuffer());
			if (format == 3)
			{
				out = QImage(res->GetHeight(), res->GetWidth(), QImage::Format_Mono);

				out.convertToFormat(QImage::Format_RGB888);
				return out;
			}
			else
			{
				out = QImage(res->GetHeight(), res->GetWidth(), QImage::Format_Mono);
				return out;
			}
		}
		catch (GenICam::GenericException& e)
		{
			if (get_count > 3)
			{
				throw CameraError(std::string("BaslerError:") + e.what());
			}
		}
	}


}


void BaslerCameraInterface::showCameraSettingDialog()
{
	BaslerCameraSettingDialog dialog(this);
	dialog.exec();
}

void BaslerCameraInterface::changeParamFromMap(const std::map<std::string, std::tuple<std::string, std::string>>&)
{
}

void BaslerCameraInterface::getParamFromCamera(std::map<std::string, std::tuple<std::string, std::string>>&)
{
}

#include "BaslerCameraInterface.moc"