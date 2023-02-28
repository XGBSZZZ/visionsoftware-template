#pragma once

#include "pylon/PylonVersion.h"
#pragma comment (lib,"GCBase_MD_VC141_v3_1_Basler_pylon")
#pragma comment (lib,"GenApi_MD_VC141_v3_1_Basler_pylon")
#pragma comment (lib,"PylonBase_v5_2")
#pragma comment (lib,"PylonC")
//#pragma comment (lib,"PylonC_BCC55")
#pragma comment (lib,"PylonGUI_v5_2")
#pragma comment (lib,"PylonUtility_v5_2")

// Include files to use the PYLON API.
#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

// Namespace for using pylon objects.
using namespace Pylon;

// Settings to use Basler GigE cameras.
#include <pylon/gige/BaslerGigEInstantCamera.h>
typedef Pylon::CBaslerGigEInstantCamera Camera_t;
using namespace Basler_GigECameraParams;

// The camera specific grab result smart pointer.
typedef Camera_t::GrabResultPtr_t GrabResultPtr_t;


#include "../CameraInterface/camerainterface.hxx"

#include <memory>
#include <mutex>
#include <QImage>

namespace Pylon {
	class CInstantCamera;
}
class BaslerCameraInterface : public CameraInterface
{

public:
	enum BaslerType
	{
		PYLON_GIGE,
		PYLON_1394,
		PYLON_USB,
		PYLON_CLSER
	};
	BaslerCameraInterface(const BaslerType&);
	~BaslerCameraInterface();
	void openCamera(const int&);
	cv::Mat getCvMat(const int&, const bool& flag = false);
	QImage getQImage(const int&, const bool& flag = false);
	bool isOpen();
	void openWithSn(const std::string&);
	std::map<std::string, int> getCameraList();
	std::string getSerialName() {
		return serial_name;
	}
	void showCameraSettingDialog();
	void changeParamFromMap(const std::map<std::string, std::tuple<std::string, std::string>>&);
    void getParamFromCamera(std::map<std::string, std::tuple<std::string, std::string>>&);
private:
	std::string serial_name;
	std::mutex mutex;
	BaslerType camera_type;
	std::shared_ptr<Camera_t> camera;
	bool camera_statu;

public:

};

