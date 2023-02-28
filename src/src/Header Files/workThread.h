#pragma once

#include "Useme.h"
#include <QFile>

#include <QThread>

#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\types_c.h> 
#include <opencv2\highgui\highgui.hpp>  //opencv申明
#include <opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h >
#include "qdir.h"
#include "qdiriterator.h"
#include "qdatetime.h"
#include "qdebug.h"	
#include "FriendFuntion.h"
#include "qlabel.h"
//#include <fileapi.h>
#include <Windows.h>


class displayimageThread : public QThread
{
	Q_OBJECT

public:
	explicit displayimageThread(QObject* parent = 0);
	~displayimageThread();
private:
	void run();
private:
	cv::Mat displayimages;
	QSize uisize;
	friend class MyWorkWindow;
	friend class MainWindows;
signals:
	void show_image_thread(const QImage&);
	void show_text_thread(const QString&);
};




class newSaveDataThread : public QThread
{
	Q_OBJECT

public:
	explicit newSaveDataThread(cv::Mat saveimage, std::string imageisoks,
		std::string saveimagepaths, bool issouces, std::string savekid,
		int savegoodnumbs, QObject* parent = 0);
	~newSaveDataThread();
	void set_dayandnight(bool isdaynight, int daytimehouse, int daytimemin, int nighttimehouse, int nighttimemin);
	void set_deleteday(bool needdelete, bool mustdelete, int maxday, int maxsize) {
		m_needdelete = needdelete;
		m_mustdelete = mustdelete;
		m_maxday = maxday;
		m_maxsize = maxsize;
	}

private:
	void run();
	//小心点用 不然可能磁盘会格式化
	void CleanOldFile(const bool& mustclear, std::string std_path, int maxday, int maxsize, unsigned int rest = 0) {
		QString path = FriendFuntion::str2qstr(std_path);
		int nowsize = 0;
		if (mustclear)
		{
			QStringList dirpath = path.split("/");
			nowsize = getDiskFreeSpace(dirpath[0]);
		}
		QString year = QDateTime::currentDateTime().toString("yyyy");
		QString today = QDateTime::currentDateTime().toString("yyyy-MM-dd"); //  yyyy表示年；MM表示月；dd表示日； hh表示小时；mm表示分；ss表示秒；zzz表示毫秒；ddd表示周几
		QDirIterator it(path, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
		std::vector<QString> sub_dir;
		while (it.hasNext()) {
			it.next();
			auto base = it.fileInfo().baseName();
			if (nowsize <= maxsize && mustclear)
			{
				sub_dir.emplace_back(path + base);
				break;
			}
			QStringList allbase = base.split(" ");
			allbase = allbase[0].split("-");
			auto nowbast = today.split("-");
			if (allbase.size() != nowbast.size()) continue;
			QDate all = QDate(allbase[0].toInt(), allbase[1].toInt(), allbase[2].toInt());
			QDate now = QDate(nowbast[0].toInt(), nowbast[1].toInt(), nowbast[2].toInt());
			auto day = now.daysTo(all);
			if (day > maxday) {
				sub_dir.emplace_back(it.fileInfo().absoluteFilePath());
			}
		}

		if (sub_dir.size() <= rest) return;
		for (int i = 0; i < sub_dir.size() - rest; i++) {
			QString path = sub_dir[i];
			// remove_directory(path);
			QDir dir;
			dir.setPath(path);
			dir.removeRecursively();
		}
	}
	quint64 getDiskFreeSpace(QString driver)
	{
		LPCSTR lpcwstrDriver = (LPCSTR)driver.utf16();
		ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
		if (!GetDiskFreeSpaceEx(lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes))
		{
			qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed.";
			return 0;
		}
		return (quint64)liTotalFreeBytes.QuadPart / 1024 / 1024 / 1024;
	}
private:
	friend class UseAlgorithm;
	std::string saveimagepath;
	cv::Mat saveimages;
	std::string imageisok = "Null";
	bool issouce;
	std::string savekind;
	int savegoodnumb;
private:
	bool m_isdaynight = false;
	int m_daytimehouse;
	int m_daytimemin;
	int m_nighttimehouse;
	int m_nighttimemin;
private:
	bool m_needdelete = false;
	bool m_mustdelete = false;
	int m_maxday;
	int m_maxsize;
};





class saveProductionThread : public QThread
{
	Q_OBJECT

public:
	explicit saveProductionThread(QObject* parent = 0);
	~saveProductionThread();
private:
	void  run();
public:
	void set_pargram(QString savepaths, bool isdaynight, int daytimehouse,
		int daytimemin, int nighttimehouse, int nighttimemin,
		QString sums, QString nices, QString oks, QString ngs);


	bool autoclear = false;
	QString nowshift = "unknow";
	bool m_isdaynight = false;
	int m_daytimehouse;
	int m_daytimemin;
	int m_nighttimehouse;
	int m_nighttimemin;
private:
	QString savepath;
	QFile Productionfile;

	QString sum;
	QString nice;
	QString ok;
	QString ng;
signals:
	void change_shifts();
};




class saveTextThread : public QThread
{
	Q_OBJECT

public:
	explicit saveTextThread(QString filepath = "", const bool& iscsv = false, QObject* parent = 0);
	~saveTextThread();

private:
	virtual void run();

public:
	std::vector<QString> savewhat;
	QString savepath;
	void set_title(const bool& savetitle, const QString& title);
	void set_dayandnight(bool isdaynight, int daytimehouse, int daytimemin, int nighttimehouse, int nighttimemin);
private:
	bool m_isdaynight = false;
	int m_daytimehouse;
	int m_daytimemin;
	int m_nighttimehouse;
	int m_nighttimemin;
	QFile Daybookfile;
	bool m_iscsv = false;

	bool m_savetitle = false;
	QString m_title = "";


signals:

};






