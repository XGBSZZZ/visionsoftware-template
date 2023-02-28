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

	QString error = "OK"; //���صĴ������

	switch (camname)
	{
	case(FriendFuntion::MyCameranames::Null):
	{
		error = tr("�����Ǳ���ȡͼģʽ");
		break;
	}
		
	case(FriendFuntion::MyCameranames::Area_Scan_Cameras):
	{
		//errorcode = Mycanmer1.open();    //������д������ĺ��� ���Ϊ����ȡͼ����Ҫд����
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
	cv::Mat return_image;  //���ڷ��ؽ��ͼ



	/////////////////////////////////////****************************/////////////////////////////////////
	//����ѡ���ĸ����

	switch (camname)
	{  

	case(FriendFuntion::MyCameranames::Null): {
		if (demo_image.size() == 0)
		{
			throw (tr("testimage�ļ���û�в���ͼƬ"));
		}
		demoimagenumb = demoimagenumb == (demo_image.size() - 1) ? 0 : demoimagenumb + 1;

		return_image = demo_image[demoimagenumb].clone();
		break;
	}

	case(FriendFuntion::MyCameranames::Line_Scan_Cameras_Dalse): {
		if (camerrun)
		{
			show_text(tr("�Զ�����ģʽ�½�ֹ���β���"));
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
	//���ڶ�ͼ������ж�

	if (return_image.empty())
	{
		qDebug() << "ͼ��Ϊ��";
		throw tr("ͼ��Ϊ��");
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
			show_text(tr("��λ��ʼ����"));
			return true;
		}
		else
		{
			show_text(tr("��λֹͣ����"));
			return false;
		}
	}
	else
	{
		show_text(tr("���δ��"));
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
		//areascancamer = new AreaScanCamer();  ʵ�������
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

