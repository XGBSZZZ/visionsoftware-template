#pragma once

#include "Useme.h"
#include <QObject>
#include <opencv2/opencv.hpp>          
#include <qfiledialog.h>                //getopenfilename 类申明
#include <opencv2\imgproc\types_c.h >
#include <qdebug.h>
#include <direct.h>
#include <io.h>
#include <string>

namespace FriendFuntion {


	extern QImage cvMat2QImage(const cv::Mat&);
	extern cv::Mat QImage2cvMat(const QImage&, const int& conn = 3);
	extern void isDirExist(const std::string filename);
	extern bool isFileExist(const std::string filename);
	extern QString str2qstr(const std::string str);
	extern std::string qstr2str(const QString qstr);


	enum MyCameranames {
		Null,
		Line_Scan_Cameras,
		Area_Scan_Cameras,
		Line_Scan_Cameras_Dalse,
		Cameras2,
		Cameras3,
	};

	enum MyAlgorithms {
		Demo,
		Demo_Defect,
		Defect,
		Measure,
		Alg1,
		Alg2,
		Alg3,
	};

	enum MyCalib {
		Area_Scan_Calib,
		Line_Scan_Calib,
		Three_Dimension_Calib,
	};


	enum Communication {
		UDP,
		TCP,

	};

	enum MyUser {
		Engineer,
		Player,  //我喜欢把他叫做玩家
	};
	// 代码声明
}