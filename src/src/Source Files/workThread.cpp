#include "Useme.h"
#include "workThread.h"
#include <qthread.h>
#include <qwaitcondition.h>
#include <QtWidgets\qwidget.h>
#include "QLoadingWidget.h"
#include <opencv2\highgui\highgui.hpp>




#define GETTIME




displayimageThread::displayimageThread(QObject* parent)
	: QThread(parent)
{

}

displayimageThread::~displayimageThread()
{
	//this->terminate();
}


void displayimageThread::run()
{
	cv::Mat displaytemp;
	displaytemp = displayimages.clone();
	//cv::imwrite("E://Dontdelete/hahaha/TF39 v1.7.0/mabe/wd.jpg", displayimages);
	//displayimage = displayimages.clone();
	//cv::imwrite("E://Dontdelete/hahaha/TF39 v1.7.0/mabe/wdn.jpg", displayimage);
	if (displaytemp.channels() == 3)
	{
		cv::cvtColor(displaytemp, displaytemp, CV_BGR2RGB);
	}
	else if (displaytemp.channels() == 1)
	{
		cv::cvtColor(displaytemp, displaytemp, CV_GRAY2RGB);
	}
	//cv::imwrite("E://Dontdelete/hahaha/TF39 v1.7.0/mabe/wdnm.jpg", displayimage);
	//cv::Mat resizeimage;
	//cv::resize(displayimage, displayimage, cv::Size(uiweight, uiheight));
	//cv::namedWindow("wd", cv::WINDOW_KEEPRATIO);
	//cv::imshow("wd", displayimage);
	//cv::waitKey(100);
	//cv::imwrite("E://Dontdelete/hahaha/TF39 v1.7.0/mabe/wdnm.jpg", displayimage);
	//qDebug() << uisize;

	QImage img = QImage((const unsigned char*)(displaytemp.data), displaytemp.cols, displaytemp.rows, displaytemp.step, QImage::Format_RGB888);
	img = img.scaled(uisize);

	//img.save("E://Dontdelete/hahaha/TF39 v1.7.0/mabe/wdnmd.jpg");
	displaytemp.release();
	emit show_image_thread(img);
	quit();
}








newSaveDataThread::newSaveDataThread(cv::Mat saveimage, std::string imageisoks,
	std::string saveimagepaths, bool issouces, std::string savekid, int savegoodnumbs, QObject* parent )
	: QThread(parent)
{
	saveimages = saveimage;
	saveimagepath = saveimagepaths;
	issouce = issouces;
	imageisok = imageisoks;
	savekind = savekid;
	savegoodnumb = savegoodnumbs;
}

newSaveDataThread::~newSaveDataThread()
{
	//qDebug() << "saveimageout";
}


void newSaveDataThread::run()
{
#ifdef SHOWALLTIME
	//double starttime = cv::getTickCount();
#endif // SHOWALLTIME
	    using namespace cv;

		if (m_isdaynight)
		{
			QString dayandnightname;

			int house = QDateTime::currentDateTime().toString("hh").toInt(); //  hh表示小时
			int min = QDateTime::currentDateTime().toString("mm").toInt(); // mm表示分
			if ((house >= m_daytimehouse && min >= m_daytimemin && house < m_nighttimehouse)
				|| (house == m_nighttimehouse && min <= m_nighttimemin))
			{
				dayandnightname = "   Day shift";
			}
			else
			{
				dayandnightname = "   Night shift";
			}


			QByteArray cdata = dayandnightname.toLocal8Bit();
			std::string daynightpath = std::string(cdata);

			cv::Mat saveimagetemp = saveimages.clone();
			QString current_datedir = QDateTime::currentDateTime().toString("yyyy-MM-dd"); //  yyyy表示年；MM表示月；dd表示日； 
			QString current_datefile = QDateTime::currentDateTime().toString("hh-mm-ss-zzz");//  hh表示小时；mm表示分；ss表示秒；zzz表示毫秒;
			FriendFuntion::isDirExist(saveimagepath + "/" + imageisok + "/" + current_datedir.toStdString() + daynightpath);
			//friendfunciotn->isDirExist(saveimagepath + "/" + imageisok + "/" + current_datedir.toStdString()+ daynightpath);
			std::string filename = issouce ? "-souce" : "-result";
			std::string path = saveimagepath + "/" + imageisok + "/" + current_datedir.toStdString() + daynightpath + "/ " + current_datefile.toStdString() + filename + savekind;
			if (savekind == ".jpg")
			{
				std::vector<int> compression_params;
				compression_params.push_back(IMWRITE_JPEG_QUALITY);  //选择jpeg
				compression_params.push_back(savegoodnumb); //在这个填入你要的图片质量
				imwrite(path, saveimagetemp, compression_params);
			}
			else if (savekind == ".png")
			{
				std::vector<int> compression_params;
				compression_params.push_back(IMWRITE_PNG_COMPRESSION);  //选择png
				compression_params.push_back(9); //在这个填入你要的图片质量
				imwrite(path, saveimagetemp, compression_params);
			}
			else if (savekind == ".bmp")
			{
				imwrite(path, saveimagetemp);
			}
			else
			{
				qDebug() << "error of savaimage";
			}


		}
		else
		{

			cv::Mat saveimagetemp = saveimages.clone();
			QString current_datedir = QDateTime::currentDateTime().toString("yyyy-MM-dd"); //  yyyy表示年；MM表示月；dd表示日； 
			QString current_datefile = QDateTime::currentDateTime().toString("hh-mm-ss-zzz");//  hh表示小时；mm表示分；ss表示秒；zzz表示毫秒;
			FriendFuntion::isDirExist(saveimagepath + "/" + imageisok + "/" + current_datedir.toStdString());
			std::string filename = issouce ? "-souce" : "-result";
			//std::string path = "workdata/" + workname + "/" + imageisok + "/" + current_datedir.toStdString() + "/ " + current_datefile.toStdString() + filename + ".jpg";

			std::string path = saveimagepath + "/" + imageisok + "/" + current_datedir.toStdString() + "/ " + current_datefile.toStdString() + filename + savekind;
			if (savekind == ".jpg")
			{
				std::vector<int> compression_params;
				compression_params.push_back(IMWRITE_JPEG_QUALITY);  //选择jpeg
				compression_params.push_back(savegoodnumb); //在这个填入你要的图片质量
				imwrite(path, saveimagetemp, compression_params);
			}
			else if (savekind == ".png")
			{
				std::vector<int> compression_params;
				compression_params.push_back(IMWRITE_PNG_COMPRESSION);  //选择png
				compression_params.push_back(9); //在这个填入你要的图片质量
				imwrite(path, saveimagetemp, compression_params);
			}
			else if (savekind == ".bmp")
			{
				imwrite(path, saveimagetemp);
			}
			else
			{
				qDebug() << "error of savaimage";
			}
		}

		if (m_needdelete || m_mustdelete)
		{
			CleanOldFile(m_mustdelete,saveimagepath + "/" + imageisok + "/", m_maxday, m_maxsize);
		}


#ifdef SHOWALLTIME
	//  double endtime = cv::getTickCount();
	  //double useTime = (endtime - starttime) / cv::getTickFrequency() * 1000;
	  //qDebug()<<(tr("存储耗时:") + QString::number(useTime, 'f', 4) + "ms");
#endif // SHOWALLTIME

	quit();
}

void newSaveDataThread::set_dayandnight(bool isdaynight, int daytimehouse, int daytimemin, int nighttimehouse, int nighttimemin)
{
	m_isdaynight = isdaynight;
	m_daytimehouse = daytimehouse;
	m_daytimemin = daytimemin;
	m_nighttimehouse = nighttimehouse;
	m_nighttimemin = nighttimemin;
}



saveProductionThread::saveProductionThread(QObject* parent)
	: QThread(parent)
{
}

saveProductionThread::~saveProductionThread()
{

}


void saveProductionThread::run()
{

	QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd"); //  yyyy表示年；MM表示月；dd表示日； hh表示小时；mm表示分；ss表示秒；zzz表示毫秒；ddd表示周几

	QString savepaths = savepath + "/ProductionStatistics/";
	QByteArray cdata = savepaths.toLocal8Bit();
	std::string path = std::string(cdata);



	savepaths += current_date;

	if (m_isdaynight)
	{
		QString dayandnightname;

		int house = QDateTime::currentDateTime().toString("hh").toInt(); //  hh表示小时
		int min = QDateTime::currentDateTime().toString("mm").toInt(); // mm表示分
		if ((house >= m_daytimehouse && min >= m_daytimemin && house < m_nighttimehouse)
			|| (house == m_nighttimehouse && min <= m_nighttimemin))
		{
			dayandnightname = "   Day shift";
		}
		else
		{
			dayandnightname = "   Night shift";
		}

		if (autoclear)
		{
			nowshift = nowshift == "unknow" ? dayandnightname : nowshift;

			if (nowshift != dayandnightname)
			{
				nowshift = dayandnightname;
				emit change_shifts();
			}

		}
	
		savepaths += dayandnightname;

	}
	else
	{
		if (autoclear)
		{
		  nowshift = nowshift == "unknow" ? current_date : nowshift;
		  if (nowshift != current_date)
		  {
			nowshift = current_date;
			emit change_shifts();
		  }
		}

	}

	Productionfile.setFileName(savepaths + ".csv");
	if (!Productionfile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
		qDebug() << "文件打开失败";
	}
	QTextStream SaveProduction(&Productionfile);
	SaveProduction << "总数:, " + sum + "\n";
	SaveProduction << "优率:, " + nice + "\n";
	SaveProduction << "OK数:, " + ok + "\n";
	SaveProduction << "NG数:, " + ng + "\n";

	



	quit();
}

void saveProductionThread::set_pargram(QString savepaths, bool isdaynight, int daytimehouse,
	int daytimemin, int nighttimehouse, int nighttimemin,
    QString sums, QString nices , QString oks, QString ngs)
{
	savepath = savepaths;
	m_isdaynight = isdaynight;
	m_daytimehouse = daytimehouse;
	m_daytimemin = daytimemin;
	m_nighttimehouse = nighttimehouse;
	m_nighttimemin = nighttimemin;
	sum = sums;
	nice = nices;
	ok = oks;
	ng = ngs;
}





saveTextThread::saveTextThread(QString filepath, const bool& iscsv, QObject* parent)
	: QThread(parent)
{
	savepath = filepath;
	m_iscsv = iscsv;
	//QString current_datedir = QDateTime::currentDateTime().toString("yyyy-MM-dd"); //  yyyy表示年；MM表示月；dd表示日；
}

saveTextThread::~saveTextThread()
{

}


void saveTextThread::run()
{

	bool havefile = false;


	QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd"); //  yyyy表示年；MM表示月；dd表示日； hh表示小时；mm表示分；ss表示秒；zzz表示毫秒；ddd表示周几
	
	QByteArray cdata = savepath.toLocal8Bit();
	std::string path = std::string(cdata);


	/// <summary>
	/// 白夜班和格式的判断
	/// </summary>
	if (m_isdaynight)
	{
		QString dayandnightname;

		int house = QDateTime::currentDateTime().toString("hh").toInt(); //  hh表示小时
		int min = QDateTime::currentDateTime().toString("mm").toInt(); // mm表示分
		if ( ( house >= m_daytimehouse && min >= m_daytimemin && house < m_nighttimehouse ) 
			|| ( house == m_nighttimehouse && min <= m_nighttimemin) )
		{
			dayandnightname = "   Day shift";
		}
		else
		{
			dayandnightname = "   Night shift";
		}
		if (m_iscsv)
		{
			havefile = FriendFuntion::isFileExist(path + current_date.toStdString() + dayandnightname.toStdString() + ".csv");
			Daybookfile.setFileName(savepath + current_date + dayandnightname + ".csv");
		}
		else
		{
			havefile = FriendFuntion::isFileExist(path + current_date.toStdString() + dayandnightname.toStdString() + ".txt");
			Daybookfile.setFileName(savepath + current_date + dayandnightname + ".txt");
		}

	}
	else
	{
		if (m_iscsv)
		{
			havefile = FriendFuntion::isFileExist(path + current_date.toStdString() + ".csv");
			Daybookfile.setFileName(savepath + current_date + ".csv");
		}
		else
		{
			havefile = FriendFuntion::isFileExist(path + current_date.toStdString() + ".txt");
			Daybookfile.setFileName(savepath + current_date + ".txt");
		}
	}

	if (!Daybookfile.open(QIODevice::Append | QIODevice::Text)) {
		qDebug() << "文件打开失败";
	}
	QTextStream SaveDaybook(&Daybookfile);

	//表头写入
	if ((!havefile || m_savetitle) && m_title.size() > 0)
	{
		m_savetitle = false;
		SaveDaybook << m_title + "\n";
	}

	//数据写入
	for (auto& i : savewhat) {
		SaveDaybook << i + "\n";
	}


	Daybookfile.close();
	quit();
}

void saveTextThread::set_title(const bool& savetitle, const QString& title)
{
	m_savetitle = savetitle;
	m_title = title.size() > 0 ? title : m_title + "";
}

void saveTextThread::set_dayandnight(bool isdaynight, int daytimehouse, int daytimemin,
	int nighttimehouse, int nighttimemin)
{
	m_isdaynight = isdaynight;
	m_daytimehouse = daytimehouse;
	m_daytimemin = daytimemin;
	m_nighttimehouse = nighttimehouse;
	m_nighttimemin = nighttimemin;
}







