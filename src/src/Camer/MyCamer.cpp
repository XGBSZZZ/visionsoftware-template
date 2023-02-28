#include "MyCamer.h"
#include <opencv2/imgproc/types_c.h>

MyCamer::MyCamer(QObject *parent)
	: QThread(parent)
{
	std::string pattern_jpg = "testimage";
	std::vector<cv::String> image_files;
	cv::glob(pattern_jpg, image_files);

	for (auto &i : image_files)
	{
		demo_image.push_back(cv::imread(i,0));
	}


}

MyCamer::~MyCamer()
{

}

void MyCamer::run()
{
	try
	{
#ifdef SHOWALLTIME
			double starttime = cv::getTickCount();
#endif // SHOWALLTIME

			cv::Mat getimage = get_image();


			buffer_image[nowbuffer] = getimage;
			emit  give_image(nowbuffer,buffer_image[nowbuffer]);
			nowbuffer = (nowbuffer + 1) == buffernumb ? 0 : nowbuffer + 1;

#ifdef SHOWALLTIME
			double endtime = cv::getTickCount();
			double useTime = (endtime - starttime) / cv::getTickFrequency() * 1000;

			//qDebug() << "get image time:" + QString::number(useTime, 'f', 4) + "ms";
#endif // SHOWALLTIME

		

	}
	catch (const std::exception& err)
	{
		show_text(err.what());
	}
	catch (const char* err) {
		QString errs(err);
		emit save_warning_message(errs);
		show_text(errs);
	}
	catch (const QString err) {
		emit save_warning_message(err);
		show_text(err);
	}
	

	
	quit();
}


QString MyCamer::open_camer()
{

	QString error = "OK"; //返回的错误代码

	switch (camname)
	{
	case(FriendFuntion::MyCameranames::Null):
	{
		error = tr("现在是本地取图模式");
		break;
	}
		
	case(FriendFuntion::MyCameranames::Area_Scan_Cameras):
	{
		//errorcode = Mycanmer1.open();    //在这里写打开相机的函数 如果为本地取图则不需要写东西
		break;
	}
	
	case(FriendFuntion::MyCameranames::Line_Scan_Cameras):
	{
		break;
	}

	case(FriendFuntion::MyCameranames::Line_Scan_Cameras_Dalse):
	{
		try
		{
			dalsecamer->openWithSn(snNumber_Dalse);
		}
		catch (const std::exception& err)
		{
			error = err.what();			
		}
		break;
	}

	case(FriendFuntion::MyCameranames::Cameras2):
	{
		break;
	}

	case(FriendFuntion::MyCameranames::Cameras3):
	{
		break;
	}


	default:

		break;

	}


	return error;
}

cv::Mat MyCamer::get_image()
{
	cv::Mat return_image;  //用于返回结果图



	/////////////////////////////////////****************************/////////////////////////////////////
	//用于选择哪个相机

	switch (camname)
	{  

	case(FriendFuntion::MyCameranames::Null): {
		if (demo_image.size() == 0)
		{
			throw (tr("testimage文件中没有测试图片"));
		}
		demoimagenumb = demoimagenumb == (demo_image.size() - 1) ? 0 : demoimagenumb + 1;

		return_image = demo_image[demoimagenumb].clone();
		break;
	}

	case(FriendFuntion::MyCameranames::Line_Scan_Cameras_Dalse): {
		if (camerrun)
		{
			show_text(tr("自动运行模式下禁止单次测试"));
		}
		else
		{
			return_image = dalsecamer->getCvMat();
		}
	break;
	}

	default: {	
		
		break;
	}
		
		

	}

	/////////////////////////////////////****************************/////////////////////////////////////
	//用于对图像进行判断

	if (return_image.empty())
	{
		qDebug() << "图像为空";
		throw tr("图像为空");
	}



	return return_image;
}

void MyCamer::set_parameters(const CamerParameters& camerpar)
{


}

void MyCamer::get_parameters(CamerParameters& camerpar)
{


}

bool MyCamer::isopen()
{
	bool camerisopen = false;



	switch (camname)
	{

	case(FriendFuntion::MyCameranames::Null): {
		camerisopen = false;
		break;
	}


	case(FriendFuntion::MyCameranames::Area_Scan_Cameras):
	{
		camerisopen = false;
		break;
	}

	case(FriendFuntion::MyCameranames::Line_Scan_Cameras):
	{
		camerisopen = false;
		break;
	}


	case(FriendFuntion::MyCameranames::Line_Scan_Cameras_Dalse):
	{
		camerisopen = dalsecamer->isOpen();
		break;
	}
	}
	return camerisopen;
}

bool MyCamer::setcamerrun(const bool& z)
{
	if (isopen())
	{
		camerrun = z;
		if (camerrun)
		{
			show_text(tr("工位开始运行"));
			return true;
		}
		else
		{
			show_text(tr("工位停止运行"));
			return false;
		}
	}
	else
	{
		show_text(tr("相机未打开"));
		return false;
	}
}

int MyCamer::Otherinit()
{
	buffer_image.resize(buffernumb);

	switch (camname)
	{

	case(FriendFuntion::MyCameranames::Null):
	{
		break;
	}

	case(FriendFuntion::MyCameranames::Area_Scan_Cameras):
	{	
		//areascancamer = new AreaScanCamer();  实例化相机
		break;
	}

	case(FriendFuntion::MyCameranames::Line_Scan_Cameras):
	{
		break;
	}

	case(FriendFuntion::MyCameranames::Line_Scan_Cameras_Dalse):
	{
		dalsecamer = new DalsaCameraInterface(ccfPath_Dalse, buffernumb);
		connect(dalsecamer, &DalsaCameraInterface::sig_give_Buffer, [=](int label,cv::Mat img) {
			if (camerrun)
			{
				emit give_image(label, img);
			}
			});
		break;
	}

	case(FriendFuntion::MyCameranames::Cameras2):
	{
		break;
	}

	case(FriendFuntion::MyCameranames::Cameras3):
	{
		break;
	}


	default:
	{
		break;
	}

	}

	return 0;
}

