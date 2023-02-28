#pragma once
#include "camerainterface.hxx"
#include <filesystem>
#include <QImage>
#ifdef QT_GUI_LIB
#include <QFileDialog>
#endif // QT_GUI_LIB
class VirtualCameraInterface : public CameraInterface
{
public:
	VirtualCameraInterface() : path("./"),iter(path)
	{

	}
	~VirtualCameraInterface(){}
	virtual bool isOpen() { return true; }
	virtual void openCamera(const int&) {};
	virtual void openWithSn(const std::string& ph) 
	{
		if (!std::filesystem::exists(ph))
		{
			std::filesystem::create_directory(ph);
		}
		path = ph;
		iter = std::filesystem::directory_iterator(path);
	}
	virtual void getParamFromCamera(std::map<std::string, std::tuple<std::string, std::string>>&)
	{

	}

	virtual cv::Mat getCvMat(const int& tp,const bool& flag = false) 
	{
		std::lock_guard<std::mutex> locker(mutex);
		cv::Mat out;
		if (iter == std::filesystem::end(iter))
		{
			iter = std::filesystem::directory_iterator(path);
		}
		if (iter != std::filesystem::end(iter))
		{
			if (*(iter->path().string().end() - 1) == 'p' || *(iter->path().string().end() - 1) == 'P' ||
				*(iter->path().string().end() - 1) == 'g' || *(iter->path().string().end() - 1) == 'G')
			{
				TAG_GetImage:
				try
				{
					out = cv::imread(iter->path().string(),tp);
					if (flag)
						cv::flip(out, out, 0);
					iter++;
					return out;
				}
				catch (const cv::Exception& e)
				{
					iter++;
					throw CameraError(e.what());
				}
			}
			else
			{
				int count(0);
				while (true)
				{
					iter++;
					if (iter == std::filesystem::end(iter))
					{
						count++;
						iter = std::filesystem::directory_iterator(path);
					}
					if (count == 2)
					{
						break;
					}
					if (*(iter->path().string().end() - 1) == 'p' || *(iter->path().string().end() - 1) == 'P' ||
						*(iter->path().string().end() - 1) == 'g' || *(iter->path().string().end() - 1) == 'G')
					{
						goto TAG_GetImage;
					}
				}
			}
		}
		if(iter != std::filesystem::end(iter))
			iter++;
		throw CameraError("Can't find image.");
		return cv::Mat();
	}
	virtual std::map<std::string, int> getCameraList() {
		return std::map<std::string, int>();
	}
	virtual void changeParamFromMap(const std::map<std::string, std::tuple<std::string, std::string>>&)
	{
	}
	virtual QImage getQImage(const int& tp, const bool& flag = false)
	{
		std::lock_guard<std::mutex> locker(mutex);
		QImage out;
		if (iter == std::filesystem::end(iter))
		{
			iter = std::filesystem::directory_iterator(path);
		}
		if (iter != std::filesystem::end(iter))
		{
			if (*(iter->path().string().end() - 1) == 'p' || *(iter->path().string().end() - 1) == 'P' ||
				*(iter->path().string().end() - 1) == 'g' || *(iter->path().string().end() - 1) == 'G')
			{
			TAG_GetImage:
				try
				{
					auto b = out.load(iter->path().string().c_str());
					auto w = out.width();
					auto h = out.height();
					auto f = out.format();
					switch (tp)
					{
					case 1:
						out = out.convertToFormat(QImage::Format_Mono);
						break;
					default:
						break;
					}
					iter++;
					return out;
				}
				catch (const cv::Exception& e)
				{
					iter++;
					throw CameraError(e.what());
				}
			}
			else
			{
				int count(0);
				while (true)
				{
					iter++;
					if (iter == std::filesystem::end(iter))
					{
						count++;
						iter = std::filesystem::directory_iterator(path);
					}
					if (count == 2)
					{
						break;
					}
					if (*(iter->path().string().end() - 1) == 'p' || *(iter->path().string().end() - 1) == 'P' ||
						*(iter->path().string().end() - 1) == 'g' || *(iter->path().string().end() - 1) == 'G')
					{
						goto TAG_GetImage;
					}
				}
			}
		}
		if(iter != std::filesystem::end(iter))
			iter++;
		throw CameraError("Can't find image.");
		return QImage();
	}

	virtual std::string getSerialName() 
	{
		return path;
	}
	virtual void showCameraSettingDialog()
	{
#ifdef QT_GUI_LIB
		auto tmp_path = QFileDialog::getExistingDirectory(nullptr, "./");
		if (tmp_path.isEmpty())
			return;
		path = tmp_path.toLocal8Bit().toStdString();
		iter = std::filesystem::directory_iterator(path);
#endif // QT_GUI_LIB
	}
private:
	std::string path;
	std::filesystem::directory_iterator iter;
	std::mutex mutex;
};

