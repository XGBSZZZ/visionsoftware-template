
#include "DalsaCameraInterface.h"
#include "SapClassBasic.h"
#include <QInputDialog>

#define tf_dalsa_new

DalsaCameraInterface::DalsaCameraInterface(const std::string& cfn, const int& imagebuffernumb) :
	is_open(false),
	acqDeviceNumber(0),
	configFilename(cfn),
	isNotSupported(FALSE),
	status(FALSE),
	acquisitionCreated(TRUE),
	acqDeviceCreated(TRUE),
	Xfer(nullptr),
	Buffers(nullptr),
	Acq(nullptr),
	Feature(nullptr),
	AcqDevice(nullptr),
	rotated_flag(false),
	nowimglabel(0),
	pushimagelabel(0),
	buffernumb(10)
{
	allimg.resize(imagebuffernumb);
}

DalsaCameraInterface::~DalsaCameraInterface()
{
	if (Xfer)
	{
		Xfer->Freeze();
		if (!Xfer->Wait(5000))
		{
			Xfer->Abort();
		}
		Xfer->Destroy();
		delete Xfer;
	}
	if (Buffers)
	{
		Buffers->Destroy();
		delete Buffers;
	}
	if (Acq)
	{
		Acq->Destroy();
		delete Acq;
	}
	if (Feature)
	{
		Feature->Destroy();
		delete Feature;
	}
    for(auto& gio_tmp : gio)
    {
        gio_tmp->Destroy();
        delete gio_tmp;
    }
	if (AcqDevice)
	{
		AcqDevice->Destroy();
		delete AcqDevice;
	}
}

bool DalsaCameraInterface::isOpen()
{
	return is_open;
}

void DalsaCameraInterface::openCamera(const int& idx)
{

}

void DalsaCameraInterface::openWithSn(const std::string& sn)
{
	int serverCount = SapManager::GetServerCount();
	int acqDeviceCount;
	int devIndex = 0;
	int acqIndex = 0;
	if (serverCount == 0)
	{
		throw CameraError(u8"没有CameraServer.");
	}
	bool flag = false;
	for (int serverAcqIndex = 0; serverAcqIndex < serverCount; serverAcqIndex++)
	{
		char serverName[CORSERVER_MAX_STRLEN];
		SapManager::GetServerName(serverAcqIndex, serverName, CORSERVER_MAX_STRLEN);
		if (std::string(serverName) == "Xtium-CL_MX4_1")
		{
			acqIndex = serverAcqIndex;
			if (int deviceCount = SapManager::GetResourceCount(serverAcqIndex, SapManager::ResourceAcq) != 0)
			{
				flag = true;
				QStringList list;
				for (int deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++)
				{
					char deviceName[CORPRM_GETSIZE(CORACQ_PRM_LABEL)];
					SapManager::GetResourceName(serverName, SapManager::ResourceAcq, deviceIndex, deviceName, sizeof(deviceName));SapManager::GetResourceName(serverName, SapManager::ResourceAcq, deviceIndex, deviceName, sizeof(deviceName));SapManager::GetResourceName(serverName, SapManager::ResourceAcq, deviceIndex, deviceName, sizeof(deviceName));
					list.push_back(deviceName);
				}
				if (list.empty())
				{
					break;
				}
				bool is_ok;
				auto item = QInputDialog::getItem(nullptr, u8"Device", u8"Device list:", list, 0, false, &is_ok);
				if(is_ok)
					acqDeviceNumber = list.indexOf(item);
				acqServerName = serverName;
				break;
			}
		}
	}
	if(!flag)
		throw CameraError(u8"未找到设备.");
/*
    int m_gioCount = SapManager::GetResourceCount(acqIndex, SapManager::ResourceGio);
	for(int i = 0;i < m_gioCount;i++)
    {
        char gioname[CORSERVER_MAX_STRLEN];
        SapManager::GetResourceName("Xtium-CL_MX4_1",SapManager::ResourceGio,i,gioname);
        SapLocation location(acqIndex, i);
		
        char label[128];

        auto m_pGio = new SapGio(location);
        m_pGio->SetCallbackInfo(GioCallback);

        gio.push_back(m_pGio);
        for (int iIO = 0; iIO < (UINT32) m_pGio->GetNumPins(); iIO++)
        {
            m_pGio->EnableCallback(iIO, SapGio::EventRisingEdge);
        }
		
    }
*/
	SapLocation loc(acqServerName.c_str(), acqDeviceNumber);
	std::filesystem::path path(configFilename);
	auto abs_path = std::filesystem::absolute(path).string();
	Acq = new SapAcquisition(loc, abs_path.c_str());
	Buffers = new SapBuffer(buffernumb, Acq);
	
	if (Acq && !*Acq && !Acq->Create())
	{
		throw CameraError(u8"创建Acquisition实例失败.");
	}
	for (int serverAcqIndex = 0; serverAcqIndex < serverCount; serverAcqIndex++)
	{
		char serverName[CORSERVER_MAX_STRLEN];
		SapManager::GetServerName(serverAcqIndex, serverName, CORSERVER_MAX_STRLEN);


		if (int device_count = SapManager::GetResourceCount(serverName, SapManager::ResourceAcqDevice) != 0)
		{
			deviceServerName = serverName;
			devIndex = 0;
		}
	}

	SapLocation loc2(deviceServerName.c_str(), 0);
	AcqDevice = new SapAcqDevice(loc2, FALSE);
	Feature = new SapFeature(deviceServerName.c_str());
	status = Feature->Create();

	if (AcqDevice && !*AcqDevice && !AcqDevice->Create())
		acqDeviceCreated = FALSE;

	//if any of the frame grabber or camera failed to initialize, end program
	if (!acquisitionCreated || !acqDeviceCreated)
		throw CameraError(u8"创建Device实例失败.");
	char modelName[64];
	char serialNumber[64];
	AcqDevice->GetFeatureValue("DeviceID", serialNumber, sizeof(serialNumber));
	if (serialNumber != sn)
	{
		throw CameraError(u8"Serial number not match.");
	}
	AcqDevice->IsFeatureAvailable("DeviceModelName", &status);
	
	if (status)
	{
		AcqDevice->GetFeatureValue("DeviceModelName", modelName, sizeof(modelName));
		if (strstr(modelName, "Genie") != 0)
		{
			isNotSupported = TRUE;
		}
	}
	else
	{
		isNotSupported = TRUE;
	}
	if (isNotSupported)
	{
		throw CameraError(modelName + std::string(u8"不支持的型号."));
	}
    /*AcqDevice->SetFeatureValue("TriggerMode", "off");
    AcqDevice->SetFeatureValue("ClConfiguration", "Base");*/
	if (Buffers && !*Buffers && !Buffers->Create())
		throw CameraError(u8"创建Buffer实例失败.");

	Xfer = new SapAcqToBuf(Acq, Buffers, AcqCallback, this);
	if (Xfer && !*Xfer && !Xfer->Create())
		throw CameraError(u8"创建Grabber实例失败.");
	Xfer->Grab();
	serial_name = sn;
	is_open = true;

}

cv::Mat DalsaCameraInterface::getCvMat(const int& cn, const bool& flag)
{
	if (!is_open)
	{
		throw CameraError(u8"未打开相机。");
	}

    std::lock_guard<std::mutex> locker(mutex);
#ifdef tf_dalsa_new
	return allimg[nowimglabel].clone();
#else	

	char trimode[64], clc[64];

	if (AcqDevice->GetFeatureValue("TriggerMode", trimode, 64))
	{
		//dalsa_logger->info("Get image locked,camera param TriggerMode:" + std::string(trimode));
	}
	else
	{
		//dalsa_logger->info("Get image locked,camera param TriggerMode:NULL");
	}
	if (AcqDevice->GetFeatureValue("ClConfiguration", clc, 64))
	{
		//dalsa_logger->info("Get image locked,camera param ClConfiguration:" + std::string(clc));
	}
	else
	{
		//dalsa_logger->info("Get image locked,camera param ClConfiguration:NULL");
	}
	if (Xfer->IsGrabbing())
	{
		//dalsa_logger->info("Get image locked,camera param IsGrabbing:TRUE");
	}
	else
	{
		//dalsa_logger->info("Get image locked,camera param IsGrabbing:FALSE");
	}

	if (flag)
	{
		if (!Xfer->Freeze())
		{
			throw CameraError(u8"未知错误。");
		}
		if (!Xfer->Wait(5000))
		{
			throw CameraError(u8"等待停止超时。");
		}
		Xfer->Snap();

		if (!Xfer->Wait(5000))
		{
			if (!Xfer->Grab())
			{
				throw CameraError(u8"重启错误。");
			}
			throw CameraError(u8"获取超时。");
		}
		if (!Xfer->Grab())
		{
			throw CameraError(u8"重启错误。");
		}
		if (img.empty())
		{
			throw CameraError(u8"图像为空。");
		}
		cv::Mat color_img;
		switch (cn)
		{
		case 1:
			return img.clone();
			break;
		case 3:
			cv::cvtColor(img, color_img, cv::COLOR_GRAY2BGR);
			return color_img;
			break;
		}

		return img.clone();
	}
	else
	{

		if (img.empty())
		{
			throw CameraError(u8"图像为空。");
		}
		if (!rotated_flag)
		{
			switch (getTranspose())
			{
			case CameraTransPose::DoNotTransPose:
				break;
			case CameraTransPose::Rotate90:
				//img = img.t();
				cv::transpose(img, img);
				break;
			default:
				break;
			}

			switch (getFlip())
			{
			case CameraFlip::DoNotFlip:
				break;
			case CameraFlip::RoundX:
				cv::flip(img, img, 0);
				break;
			case CameraFlip::RoundY:
				cv::flip(img, img, 1);
				break;
			case CameraFlip::Both:
				cv::flip(img, img, -1);
				break;
			default:
				break;
			}
		}
		rotated_flag = true;
		cv::Mat color_img;
		switch (cn)
		{
		case 1:
			return img.clone();
			break;
		case 3:
			cv::cvtColor(img, color_img, cv::COLOR_GRAY2BGR);
			return color_img.clone();
			break;
		}


		return img.clone();
	}
#endif // tf_dalsa_new
}

QImage DalsaCameraInterface::getQImage(const int&, const bool& flag)
{
	return QImage();
}

std::map<std::string, int> DalsaCameraInterface::getCameraList()
{
	return std::map<std::string, int>();
}

std::string DalsaCameraInterface::getSerialName()
{
	return serial_name;
}

void DalsaCameraInterface::showCameraSettingDialog()
{
}

void DalsaCameraInterface::changeParamFromMap(const std::map<std::string, std::tuple<std::string, std::string>>& mp)
{
	if (AcqDevice)
	{

		for (auto& node : mp)
		{
			BOOL status;
            if(!Xfer->IsGrabbing())
            {
                Xfer->Grab();
            }
			AcqDevice->IsFeatureAvailable(node.first.c_str(), &status);
			if (status)
			{
				AcqDevice->GetFeatureInfo(node.first.c_str(), Feature);
				if (std::get<0>(node.second) == "float")
				{
					double min_val,max_val;
					Feature->GetMin(&min_val);
					Feature->GetMax(&max_val);
					if (min_val > std::atof(std::get<1>(node.second).c_str()))
					{
					    if(node.first == "ExposureTime")
                        {
					        if(exp_time_tmp == min_val)
                            {
					            continue;
                            }
					        else
                            {
					            exp_time_tmp = min_val;
                            }
                        }
					    if(node.first == "Gain")
                        {
                            if(gain_tmp == min_val)
                            {
                                continue;
                            }
                            else
                            {
                                gain_tmp = min_val;
                            }
                        }
						AcqDevice->SetFeatureValue(node.first.c_str(), min_val);

					}
					else if (max_val < std::atof(std::get<1>(node.second).c_str()))
					{
                        if(node.first == "ExposureTime")
                        {
                            if(exp_time_tmp == max_val)
                            {
                                continue;
                            }
                            else
                            {
                                exp_time_tmp = max_val;
                            }
                        }
                        if(node.first == "Gain")
                        {
                            if(gain_tmp == max_val)
                            {
                                continue;
                            }
                            else
                            {
                                gain_tmp = max_val;
                            }
                        }
						AcqDevice->SetFeatureValue(node.first.c_str(), max_val);
					}
					else
					{
                        if(node.first == "ExposureTime")
                        {
                            if(exp_time_tmp == std::atof( std::get<1>(node.second).c_str()))
                            {
                                continue;
                            }
                            else
                            {
                                exp_time_tmp = std::atof( std::get<1>(node.second).c_str());
                            }
                        }
                        if(node.first == "Gain")
                        {
                            if(gain_tmp == std::atof( std::get<1>(node.second).c_str()))
                            {
                                continue;
                            }
                            else
                            {
                                gain_tmp = std::atof( std::get<1>(node.second).c_str());
                            }
                        }
						AcqDevice->SetFeatureValue(node.first.c_str(), std::get<1>(node.second).c_str());
					}
				}
			}
		}
	}
}

void DalsaCameraInterface::getParamFromCamera(std::map<std::string, std::tuple<std::string, std::string>>& mp)
{
	if (mp.empty())
	{
		mp = std::map<std::string, std::tuple<std::string, std::string>>{
			{"ExposureTime",std::make_tuple("float","")},
			{"Gain",std::make_tuple("float","")}
		};
	}
	if (AcqDevice)
	{
		for (auto& node : mp)
		{
			BOOL status;
			AcqDevice->IsFeatureAvailable(node.first.c_str(),&status);
			if (status)
			{
				char str_rtn[128];
				AcqDevice->GetFeatureValue(node.first.c_str(), str_rtn, sizeof(str_rtn));
				std::get<1>(node.second) = std::string(str_rtn);
			}
		}
	}
}

void DalsaCameraInterface::AcqCallback(SapXferCallbackInfo* pInfo)
{
    DalsaCameraInterface* cam_ptr =static_cast<DalsaCameraInterface*>(pInfo->GetContext());
    std::lock_guard<std::mutex> locker(cam_ptr->mutex);
    //dalsa_logger->info("callback:AcqCallback locked");
	uchar pData;
	void* pDataAddr = &pData;  //将pData的地址给 *pDataAddr
	cam_ptr->Buffers->GetAddress(&pDataAddr);  //将 *pDataAddr的地址传进去  这个函数调用后Buffers里面就有数据了
	try
	{
        cam_ptr->allimg[cam_ptr->nowimglabel].release(); //将图像清空
		//将Buffer里面的数据取出来
		cam_ptr->allimg[cam_ptr->nowimglabel] = cv::Mat(cam_ptr->Buffers->GetHeight(), cam_ptr->Buffers->GetWidth(), CV_8U, pDataAddr);

		emit cam_ptr->sig_give_Buffer(cam_ptr->pushimagelabel,cam_ptr->allimg[cam_ptr->nowimglabel]);
		cam_ptr->nowimglabel = (cam_ptr->nowimglabel + 1) == cam_ptr->allimg.size() ? 0 : cam_ptr->nowimglabel + 1;
		cam_ptr->pushimagelabel = (cam_ptr->pushimagelabel + 1) == 100 ? 0 : cam_ptr->pushimagelabel + 1;
	}
	catch (const cv::Exception& e)
	{
		cam_ptr->allimg[cam_ptr->nowimglabel] = cv::Mat();
	}

   cam_ptr->Buffers->ReleaseAddress(pDataAddr);  //释放Buffer的地址

   cam_ptr->rotated_flag = false;
}

void DalsaCameraInterface::GioCallback(SapGioCallbackInfo *pInfo)
{
   // dalsa_logger->info("gio rising edge:" + std::to_string(pInfo->GetPinNumber()));
}
