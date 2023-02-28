#include "UseAlgorithm.h"
#include "workThread.h"

UseAlgorithm::UseAlgorithm(QObject *parent)
	: QThread(parent)
{
	alooutputdata.aloresult.resize(20);
}

UseAlgorithm::~UseAlgorithm()
{

}


void UseAlgorithm::run()
{

#ifdef SHOWALLTIME
	double starttime = cv::getTickCount();
#endif // SHOWALLTIME

	//判断是否有原图
	if (aloinputdata.inputimage.empty())
	{
		alooutputdata.outputimage = cv::imread("myfile/empty.png");
	}

	//初始化
	alooutputdata.errcode = "OK";
	errlabel = 0;
	for (auto &i : alooutputdata.aloresult)
	{
		i = -999999;
	}
	alooutputdata.outputimage = cv::Mat();

	//将原图丢入缓存   
	souce_buffer[nowsouce] = aloinputdata.inputimage; errlabel++;
	try
	{
		useAlg();
	}
	catch (const char* err) {
		QString errs(err);
		alooutputdata.errcode = errs;
	}
	catch (const QString err) {
		alooutputdata.errcode = err;
	}


	//判断是否有结果图
	if (alooutputdata.outputimage.empty())
	{
		alooutputdata.outputimage = cv::imread("myfile/empty.png");
	}

	//将结果图丢入缓存 
	result_buffer[nowresult] = alooutputdata.outputimage; errlabel++;
	give_data(alooutputdata);  //将结果传回工位类  MyWorkWindow	

			/////////////////////////////////////***********开启存储线程***********/////////////////////////////////////
		if (errlabel == 2)
		{
			std::string isok = alooutputdata.errcode == "OK" ? "OK" : "NG";
			if (isok == "OK")
			{
				if (saveoksouce)
				{
					emit sava_image(souce_buffer[nowsouce], isok, true);
				}
				if (saveokresult)
				{
					emit sava_image(result_buffer[nowsouce], isok, false);
				}
			}
			else if (isok == "NG")
			{
				if (savengsouce)
				{
					emit sava_image(souce_buffer[nowsouce], isok, true);
				}
				if (savengresult)
				{
					emit sava_image(result_buffer[nowsouce], isok, false);
				}
			}

			nowsouce = (nowsouce + 1) == souce_buffer.size() ? 0 : nowsouce + 1;
			nowresult = (nowresult + 1) == result_buffer.size() ? 0 : nowresult + 1;
		}	
		else
		{
			qDebug() << "err of Aloimage low numb";
			emit save_warning_message(tr("错误:算法线程缓存区溢出,请增加缓存区再重试"));
		}

#ifdef SHOWALLTIME
		double endtime = cv::getTickCount();
		double useTime = (endtime - starttime) / cv::getTickFrequency() * 1000;
		qDebug() << ("use alo time:" + QString::number(useTime, 'f', 4) + "ms");
#endif // SHOWALLTIME
		   
	quit();

}





void UseAlgorithm::useAlg()
{
	/////////////////////////////////////**********这里用于对于算法的选择*********/////////////////////////////////////

	switch (myalgrithms)
	{

	case(FriendFuntion::MyAlgorithms::Demo):
	{

			//demoinputdata.inputimage = TF1->runUnet(demoinputdata.inputimage)[0]; //深度学习算法
		    QThread::msleep(100);
		    useopencv->Demoofopencv(aloinputdata, alooutputdata);
			break;
	}
	case(FriendFuntion::MyAlgorithms::Defect):
	{
		if (aloinputdata.inputimage.channels() != 1)
		{
			throw(tr("算法输入图不为单通道"));
		}

		cv::Mat img_je, img_bt, result_er_img,result_bt_img, resultimage;


		useopencv->Defect_Roi_Opencv(aloinputdata, img_je,img_bt, result_er_img, result_bt_img, resultimage);

		
		int bt_unknowpoint = 0;
		int bt_errpoint = 0;
		double bt_max_area = 0;
		double bt_max_width = 0;
		double bt_max_height = 0;

		int er_unknowpoint = 0;
		int er_errpoint = 0;
		double er_max_area = 0;
		double er_max_width = 0;
		double er_max_height = 0;



		//这就是传说中的多线程减速吗
//		omp_set_num_threads(2);
//#pragma omp parallel for   
		for (int defect_i = 0; defect_i < 2; defect_i++)
		{
			if (defect_i == 0)
			{
				cv::Mat defect_bt_image = Defect_bt->runUnet(img_bt)[0];
				if (useopencv->display)
				{
					std::cout << "这是我的线程编号:   " << omp_get_thread_num() << std::endl;
					cv::namedWindow("本体缺陷图", CV_WINDOW_KEEPRATIO);
					cv::imshow("本体缺陷图", defect_bt_image);
				}
				useopencv->Defect_defect(true,result_bt_img, defect_bt_image, aloinputdata, bt_unknowpoint, bt_errpoint,
					bt_max_area, bt_max_width, bt_max_height);

			}
			else if (defect_i == 1)
			{
				cv::Mat defect_je_image = Defect_er->runUnet(img_je)[0];
				if (useopencv->display)
				{
					std::cout << "这是我的线程编号:   " << omp_get_thread_num() << std::endl;
					cv::namedWindow("极耳缺陷图", CV_WINDOW_KEEPRATIO);
					cv::imshow("极耳缺陷图", defect_je_image);
				}
				useopencv->Defect_defect(false,result_er_img, defect_je_image, aloinputdata,er_unknowpoint,er_errpoint,
					er_max_area,er_max_width, er_max_height);
			}
		}


		int textsize = aloinputdata.aloparmetric[10] < 0 ? 13 : aloinputdata.aloparmetric[10];

		if (bt_errpoint == 0 && er_errpoint == 0)
		{
			cv::putText(resultimage, "OK", cv::Point(300, 200), cv::FONT_HERSHEY_PLAIN, textsize, cv::Scalar(0, 255, 0), 9, 8);
		}
		else
		{
			cv::putText(resultimage, "NG", cv::Point(300, 200), cv::FONT_HERSHEY_PLAIN, textsize, cv::Scalar(0, 0, 255), 9, 8);
		}

		if (useopencv->display)
		{
			std::cout << "未知缺陷个数： " << alooutputdata.aloresult[0] << std::endl;
			std::cout << "错误缺陷个数： " << alooutputdata.aloresult[1] << std::endl;

			cv::namedWindow("缺陷图", CV_WINDOW_KEEPRATIO);
			cv::imshow("缺陷图", resultimage);
		}

		alooutputdata.aloresult[0] = bt_errpoint + er_errpoint;
		alooutputdata.aloresult[1] = bt_unknowpoint + er_unknowpoint;
		alooutputdata.aloresult[2] = bt_max_area;
		alooutputdata.aloresult[3] = bt_max_width;
		alooutputdata.aloresult[4] = bt_max_height;
		alooutputdata.aloresult[5] = er_max_area;
		alooutputdata.aloresult[6] = er_max_width;
		alooutputdata.aloresult[7] = er_max_height;



		alooutputdata.errcode = "NG";
		alooutputdata.outputimage = resultimage;
		break;
	}
		

	case(FriendFuntion::MyAlgorithms::Measure):
	{
		break;
	}


	case(FriendFuntion::MyAlgorithms::Alg1):
	{
		break;

	}


	case(FriendFuntion::MyAlgorithms::Alg2):
	{
		break;

	}


	case(FriendFuntion::MyAlgorithms::Alg3):
	{

		break;
	}
		
	default: 
		break;

	}

		
	/////////////////////////////////////**********结束**********/////////////////////////////////////
}

void UseAlgorithm::PbInit()
{
	cv::Mat loadingimage = cv::imread("myfile/tf_loadingimage.png", 0);
	Defect_bt->runUnet(loadingimage);
	Defect_er->runUnet(loadingimage);
}

void UseAlgorithm::otherInit(const int& buffernumb)
{
	souce_buffer.resize(buffernumb);
	result_buffer.resize(buffernumb);
}
