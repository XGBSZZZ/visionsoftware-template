#include "stdafx.h"
#include "BaumerCameraInterface.h"
#include "bgapi2_genicam.hpp"
#include <thread>
#include "MiCore/miLog.h"
using namespace BGAPI2;
BaumerCameraInterface::BaumerCameraInterface() :
	pDevice(nullptr),
	pSystem(nullptr),
	pInterface(nullptr),
	pDataStream(nullptr)
{
}


BaumerCameraInterface::~BaumerCameraInterface()
{
	if (!is_opened)
		return;
	try
	{
		pDevice->GetRemoteNode("AcquisitionStop")->Execute();
		pDataStream->StopAcquisition();
		auto bufferList = pDataStream->GetBufferList();
		bufferList->DiscardAllBuffers();
		while (bufferList->size() > 0)
		{
			auto pBuffer = bufferList->begin()->second;
			bufferList->RevokeBuffer(pBuffer);
			delete pBuffer;
		}
		pDataStream->Close();
		pDevice->Close();

		pInterface->Close();
		pSystem->Close();
		BGAPI2::SystemList::ReleaseInstance();

	}
	catch (...)
	{

	}

}

bool BaumerCameraInterface::is_open()
{
	return is_opened;
}

void BaumerCameraInterface::open_camera(const int &)
{
}

void BaumerCameraInterface::openWithSn(const std::string & sn, const int &)
{
	serial_name = sn;
	try
	{
		if (pDataStream != nullptr)
		{
			pDataStream->Close();
		}
		if (pDevice != nullptr)
		{
			pDevice->Close();
		}
		if (pInterface != nullptr)
		{
			pInterface->Close();
		}
		if (pSystem != nullptr)
		{
			pSystem->Close();
		}
		SystemList *systemList = SystemList::GetInstance();
		systemList->Refresh();

		for (auto system_iter : *systemList) {
			pSystem = system_iter->second;  	//gige: begin(); 
											  //System *pSystem = systemList->begin()++->second;  //usb3: begin()++
			if (!pSystem->IsOpen())
				pSystem->Open();

			InterfaceList *interfaceList = pSystem->GetInterfaces();
			interfaceList->Refresh(100);

			for (auto interface_iter : *interfaceList) {
				pInterface = interface_iter->second;		  //first  interface
														  //Interface *pInterface = interfaceList->begin()++->second;	  //second interface
														  //Interface *pInterface = interfaceList->begin()++++->second; //third  interface

				if (!pInterface->IsOpen())
					pInterface->Open();

				DeviceList *deviceList = pInterface->GetDevices();
				deviceList->Refresh(100);
				for (auto device_iter : *deviceList)
				{
					auto device = device_iter->second;

					if (device->GetSerialNumber().get() == sn) {
						pDevice = device;
						pDevice->Open();


						//pDevice->GetRemoteNode("ChunkEnable")->SetBool(false);
						pDevice->GetRemoteNode("TriggerMode")->SetString("On");
						pDevice->GetRemoteNode("TriggerSource")->SetString("SoftwareTrigger");

						DataStreamList *datastreamList = pDevice->GetDataStreams();
						datastreamList->Refresh();
						int cnt = datastreamList->size();
						pDataStream = datastreamList->begin()->second;
						pDataStream->Open();

						BGAPI2::BufferList *bufferList = pDataStream->GetBufferList();
						Buffer * pBuffer = NULL;
						for (int i = 0; i < 1; i++)
						{
							pBuffer = new BGAPI2::Buffer();
							bufferList->Add(pBuffer);
							pBuffer->QueueBuffer();
						}
						pDataStream->StartAcquisitionContinuous();
						pDevice->GetRemoteNode("AcquisitionStart")->Execute();
						is_opened = true;
						return;
					}
				}
			}
		}

	}
	catch (BGAPI2::Exceptions::ResourceInUseException& ex)
	{
		throw CameraError(ex.GetErrorDescription().get());
	}
	catch (BGAPI2::Exceptions::ObjectInvalidException& ex)
	{
		throw CameraError(ex.GetErrorDescription().get());
	}
	catch (BGAPI2::Exceptions::LowLevelException& ex)
	{
		throw CameraError(ex.GetErrorDescription().get());
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		throw CameraError(ex.GetErrorDescription().get());
	}
}

cv::Mat BaumerCameraInterface::get_cvmat(const int & type,const bool& flag)
{
	cv::Mat dst;
	try
	{

		if (!is_opened)
			return cv::Mat();
		try
		{
			pDevice->GetRemoteNode("TriggerSoftware")->Execute();
		}
		catch (...)
		{
		}
		Buffer *pBufferFilled = pDataStream->GetFilledBuffer(1000);
		if (pBufferFilled == NULL) {
			MILOG_WARN("MiBaumerCamera", "Error: Buffer Timeout after 1000 msec");
		}
		else if (pBufferFilled->GetIsIncomplete() == true) {
			MILOG_WARN("MiBaumerCamera", "Error: Image is incomplete");
			pBufferFilled->QueueBuffer();
		}
		else {
			MILOG_INFO("MiBaumerCamera", " Image " << pBufferFilled->GetFrameID() << " received. data: "
				<< pBufferFilled->GetMemPtr() << " size: " << pBufferFilled->GetWidth() << " * " << pBufferFilled->GetHeight());
			cv::Mat img(pBufferFilled->GetHeight(), pBufferFilled->GetWidth(), CV_8UC1, pBufferFilled->GetMemPtr());
			dst.create(img.size(), img.type());
			img(cv::Rect(0, 0, 264, img.rows)).copyTo(dst(cv::Rect(img.cols - 264, 0, 264, img.rows)));
			img(cv::Rect(264, 0, img.cols - 264, img.rows)).copyTo(dst(cv::Rect(0, 0, img.cols - 264, img.rows)));
			pBufferFilled->QueueBuffer();
		}
		return dst;
	}
	catch (BGAPI2::Exceptions::ResourceInUseException& ex)
	{
		throw CameraError(ex.GetErrorDescription().get());
	}
	catch (BGAPI2::Exceptions::ObjectInvalidException& ex)
	{
		throw CameraError(ex.GetErrorDescription().get());
	}
	catch (BGAPI2::Exceptions::LowLevelException& ex)
	{
		throw CameraError(ex.GetErrorDescription().get());
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		throw CameraError(ex.GetErrorDescription().get());
	}
	return dst;
}

std::map<std::string, int> BaumerCameraInterface::getCameraList()
{
	return std::map<std::string, int>();
}

std::string BaumerCameraInterface::getSerialName()
{
	return serial_name;
}
