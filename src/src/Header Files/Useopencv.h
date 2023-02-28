#pragma once


#include "Useme.h"

#include <QObject>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/photo.hpp>
	


struct Aloinputdata
{
	cv::Mat inputimage;  //输入图像 原图 用于跑算法和保存
	double scalex = 1.0; //X轴像素转换比
	double scaley = 1.0; //Y轴像素转换比

	std::vector<double> aloparmetric;  //由参数界面の算法输入 从左往右 从上往下 从0开始对应
};


struct Alooutputdata
{
	cv::Mat outputimage;  //输出图像 结果图 用于显示和保存
	QString errcode;  //错误代码  输出 OK 或 错误代码
	int label;  //由采图线程设置的标签 不用管

	std::vector<double> aloresult;  //目前设计成20个   由参数界面の算法输出 从左往右 从上往下 从0开始对应   

};

	

class Useopencv : public QObject
{
	Q_OBJECT

public:
	explicit  Useopencv(QObject *parent = 0);
	~Useopencv();
public:
	void Demoofopencv(const Aloinputdata&  , Alooutputdata& );
	void Demo_range(const Aloinputdata&, Alooutputdata&);
	void Demo_defect(const Aloinputdata&, Alooutputdata&);
	void Defect_Roi_Opencv(const Aloinputdata&,cv::Mat& jier,cv::Mat& benti, cv::Mat& resultjier, cv::Mat& resultbenti, cv::Mat& resultimage);
	void Defect_defect(const bool& isbt,cv::Mat& cut, const cv::Mat& black, const Aloinputdata& inputdata,
		int& unknowpoint, int& errpoint, double& max_area, double& max_width, double& max_height);
public:
	int demong = 0;
public:
	bool display = false; //是否显示过程
private:
	inline bool static int_max(int x, int y) {
		return x > y;
	}
	inline bool static size_max(std::vector<cv::Point> x, std::vector<cv::Point> y) {
		return x.size() > y.size();
	}
	//平均灰度值
	inline double mean_value(cv::Mat src) {
		cv::Scalar _temp_value = cv::mean(src);
		double _roi_mean = _temp_value.val[0];
		return _roi_mean;
	}
	//拟合线
	void Fitline(std::vector<cv::Point> a, cv::Mat& b, double& rotate) {
		//构建A矩阵 
		int N = 2;
		cv::Mat A = cv::Mat::zeros(N, N, CV_64FC1);
//#pragma omp parallel for
		for (int row = 0; row < A.rows; row++)
		{
			for (int col = 0; col < A.cols; col++)
			{
				for (int k = 0; k < a.size(); k++)
				{
					A.at<double>(row, col) = A.at<double>(row, col) + pow(a[k].x, row + col);
				}
			}
		}
		//构建B矩阵
//#pragma omp parallel for
		cv::Mat B = cv::Mat::zeros(N, 1, CV_64FC1);
		for (int row = 0; row < B.rows; row++)
		{

			for (int k = 0; k < a.size(); k++)
			{
				B.at<double>(row, 0) = B.at<double>(row, 0) + pow(a[k].x, row) * a[k].y;
			}
		}
		//A*X=B
		cv::Mat X;
		//std::cout << A << std::endl << B << std::endl;
		solve(A, B, X, cv::DECOMP_LU);
		std::cout << X << std::endl;
		std::vector<cv::Point>lines;
		for (int x = 0; x < b.size().width; x++)
		{				// y = b + ax;
			double y = X.at<double>(0, 0) + X.at<double>(1, 0) * x;
			/*     printf("(%d,%lf)\n", x, y);*/
			lines.push_back(cv::Point(x, y));
		}
		polylines(b, lines, false, cv::Scalar(255, 0, 0), 3, 8);

		double dy = double(lines[b.cols - 1].y) - double(lines[0].y);
		double dx = b.cols - 1 - 0;
		rotate = atan2(dy, dx);
	}
	//角度矫正
	inline void rotateImage(cv::Mat& src, cv::Mat& dst, double angle)
	{
		cv::Point2f  center(src.cols / 2, src.rows / 2);
		cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1);
		cv::Rect box = cv::RotatedRect(center, src.size(), angle).boundingRect();
		cv::warpAffine(src, dst, rot, box.size());

	}
	//找轮廓和中心点
	void Findmidandboundary(cv::Mat matSrc, std::vector<std::vector<cv::Point> >& contours, std::vector<cv::Point>& mc, cv::Mat& drawing)
	{
		std::vector<cv::Vec4i> hierarchy;
		std::vector<std::vector<cv::Point> > contourstemp;
		//寻找轮廓，这里注意，findContours的输入参数要求是二值图像，二值图像的来源大致有两种，第一种用threshold，第二种用canny
		findContours(matSrc.clone(), contours, hierarchy, 1, 1, cv::Point(0, 0));

		// 计算矩
		std::vector<cv::Moments> mu(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mu[i] = moments(contours[i], false);
		}

		mc.resize(contours.size());
		///  计算中心矩:
		for (int i = 0; i < contours.size(); i++)
		{
			mc[i] = cv::Point(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
			/* std::std::cout << mc[i] << std::std::endl;*/
			std::cout << "this is my area" << contours[i].size() << std::endl;
		}

		/// 绘制轮廓
		for (int i = 0; i < contours.size(); i++)
		{
			cv::Scalar color = cv::Scalar(255);
			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
			circle(drawing, mc[i], 4, color, -1, 8, 0);
		}

	}
	//单通道转三通道
	cv::Mat convertTo3Channels(const cv::Mat& binImg)
	{
		cv::Mat three_channel = cv::Mat::zeros(binImg.rows, binImg.cols, CV_8UC3);
		std::vector<cv::Mat> channels;
		for (int i = 0; i < 3; i++)
		{
			channels.push_back(binImg);
		}
		cv::merge(channels, three_channel);
		return three_channel;
	}
};
