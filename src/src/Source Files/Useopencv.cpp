#include "Useme.h"
#include "Useopencv.h"


using namespace std;
using namespace cv;



Useopencv::Useopencv(QObject *parent)
	: QObject(parent)
{
}

Useopencv::~Useopencv()
{
}

void Useopencv::Demoofopencv(const Aloinputdata& demoinputdata, Alooutputdata& demooutputdata)
{
     
	cvtColor(demoinputdata.inputimage, demooutputdata.outputimage, CV_GRAY2BGR);
	for (int i = 0; i < demooutputdata.aloresult.size(); i++)
	{
		demooutputdata.aloresult[i] = (i + 1) + i / static_cast<double>(10) + i / static_cast<double>(100) + i / static_cast<double>(1000);
	}

	if (demong <= 2)
	{
		putText(demooutputdata.outputimage, "OK", cv::Point(500, 300), cv::FONT_HERSHEY_PLAIN, 13, cv::Scalar(0, 255, 0), 11, 8);
		++demong;
	
	}
	else
	{
		
		putText(demooutputdata.outputimage, "NG", cv::Point(500, 300), cv::FONT_HERSHEY_PLAIN, 13, cv::Scalar(0, 0, 255), 11, 8);
		demong = 0;
		 throw ("我不管你有什么错，就是有错");
	}

	if (demoinputdata.inputimage.empty())
	{
		throw tr("图像为空");
	}

}

void Useopencv::Demo_range(const Aloinputdata&, Alooutputdata&)
{
}

void Useopencv::Demo_defect(const Aloinputdata&, Alooutputdata&)
{
}

void Useopencv::Defect_Roi_Opencv(const Aloinputdata& defectinputdata, cv::Mat& jier, cv::Mat& benti,cv::Mat& resultjier,cv::Mat& resultbenti,cv::Mat& resultimage)
{
	const int sizee = 1;  //显示轮廓大小
	const int thres_bt = 25; //本体的阈值分割
	const int thres_er = 85; //极耳的阈值分割
	const int thres_er_good = 10; //极耳精提取的阈值分割
	const int add_er_x = 3; //极耳的粗提取x轴扩展  左扩右不扩 符号看象限
	const int add_bt_x = 1; //本体的x轴扩展  左右扩展
	const int add_bt_y = 9; //本体的y轴扩展  上下扩展
	const int add_er_x_good = 1; //极耳的精提取x轴扩展  左右扩
	const int add_er_y_good = 2; //极耳的精提取y轴扩展  上下扩
	const int er_min = 3; //极耳面积最小值
	const double sca = 256; //精度与速度的平衡  虽然是double类型 但是不能有小数点  不调  不要调 

	cv::Mat souce = defectinputdata.inputimage;


	//缩放因子
	double roi_resizex = double(double(souce.cols) / sca);
	double roi_resizey = double(double(souce.rows) / sca);


	Mat resize_souce;
	cv::resize(souce, resize_souce, Size(sca, sca));

	if (display)
	{
		cv::namedWindow("原图", WINDOW_KEEPRATIO);
		cv::imshow("原图", souce);
	}


	Mat th_all;
	threshold(resize_souce, th_all, thres_bt, 255, THRESH_BINARY);

	Mat th_er;
	threshold(resize_souce, th_er, thres_er, 255, THRESH_BINARY);

	std::vector<std::vector<cv::Point>> contours_er;
	std::vector<std::vector<cv::Point>> contours_bt;
	std::vector<std::vector<cv::Point>> contours_er_good;

	findContours(th_er, contours_er, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
	sort(contours_er.begin(), contours_er.end(), size_max);  //1ms

	if (display)
	{
		Mat displayimage1;
		cvtColor(resize_souce, displayimage1, CV_GRAY2BGR);

		polylines(displayimage1, contours_er[0], true, Scalar(0, 0, 255), sizee, 8, 0);


		findContours(th_all, contours_bt, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
		sort(contours_bt.begin(), contours_bt.end(), size_max);  //1ms
		//drawContours(displayimage1, contours[0], -1, Scalar(0, 0, 255), 17);
		polylines(displayimage1, contours_bt[0], true, Scalar(0, 255, 0), sizee, 8, 0);
		//namedWindow("整体的阈值分割", WINDOW_KEEPRATIO);
		//imshow("整体的阈值分割", displayimage1);

		//drawContours(displayimage1, contours[0], -1, Scalar(0, 0, 255), 17);
		cv::namedWindow("阈值分割", WINDOW_KEEPRATIO);
		cv::imshow("阈值分割", displayimage1);
	}

	//极耳粗ROI  先提粗再提精   粗的用于 1.获得极耳粗ROI 2.用于本体ROI的提取
	cv::Mat roi_er = cv::Mat::zeros(256, 256, CV_8UC1);
	cv::Mat roi_er_good = cv::Mat::zeros(256, 256, CV_8UC1);
	cv::Rect er_rec = boundingRect(contours_er[0]);

	const int add_er_y = 99999; //   极耳的y轴扩展  上下扩展  
	er_rec.x = (er_rec.x - add_er_x) > 0 ? (er_rec.x - add_er_x) : 0;
	er_rec.width = ((er_rec.x + er_rec.width) + add_er_x) < roi_er.cols ? (er_rec.width + add_er_x) : (roi_er.cols - er_rec.x);
	er_rec.y = (er_rec.y - add_er_y) > 0 ? (er_rec.y - add_er_y) : 0;
	er_rec.height = ((er_rec.y + er_rec.height) + 2 * add_er_y) < roi_er.rows ? (er_rec.height + 2 * add_er_y) : (roi_er.rows - er_rec.y);
	rectangle(roi_er, er_rec, Scalar(255, 255, 255), CV_FILLED, 8, 0);

	if (display)
	{
		cv::namedWindow("粗极耳黑白图", WINDOW_KEEPRATIO);
		cv::imshow("粗极耳黑白图", roi_er);
	}



	//极耳精ROI
	cv::Mat low_er_img = resize_souce(er_rec);
	Mat th_er_good;
	threshold(low_er_img, th_er_good, thres_er_good, 255, THRESH_BINARY);
	findContours(th_er_good, contours_er_good, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
	sort(contours_er_good.begin(), contours_er_good.end(), size_max);  //1ms

	for (int wd = 1; wd < (contours_er_good.size() - 1); wd++)
	{
		if (display)
		{
			cout << contours_er_good.size() << endl;
		}
		if (contours_er_good[wd].size() >= er_min)
		{
			for (auto& nmd : contours_er_good[wd])
			{
				contours_er_good[0].push_back(nmd);
			}

		}
	}


	cv::Rect er_rec_good = boundingRect(contours_er_good[0]);
	//const int add_er_x_good = 3; //极耳的精提取x轴扩展  左扩右不扩 符号看象限
	//const int add_er_y_good = 0; //极耳的精提取y轴扩展  上下扩
	er_rec_good.x += er_rec.x;
	er_rec_good.x = (er_rec_good.x - add_er_x_good) > 0 ? (er_rec_good.x - add_er_x_good) : 0;
	er_rec_good.width = ((er_rec_good.x + er_rec_good.width) + 2 * add_er_x_good) < roi_er.cols ? (er_rec_good.width + 2 * add_er_x_good) : (roi_er.cols - er_rec_good.x);
	er_rec_good.y = (er_rec_good.y - add_er_y_good) > 0 ? (er_rec_good.y - add_er_y_good) : 0;
	er_rec_good.height = ((er_rec_good.y + er_rec_good.height) + 2 * add_er_y_good) < roi_er.rows ? (er_rec_good.height + 2 * add_er_y_good) : (roi_er.rows - er_rec_good.y);
	rectangle(roi_er_good, er_rec_good, Scalar(255, 255, 255), CV_FILLED, 8, 0);


	//粗极耳截图  调参用
	if (display)
	{
		cv::Mat img_er_low = low_er_img.clone();
		cv::Mat display_er_low;
		cvtColor(img_er_low, display_er_low, CV_GRAY2BGR);
		rectangle(display_er_low, er_rec_good, Scalar(255, 0, 0), 1, 8, 0);
		cv::namedWindow("精极耳阈值图", WINDOW_KEEPRATIO);
		cv::imshow("精极耳阈值图", display_er_low);
	}




	if (display)
	{
		cv::namedWindow("精极耳黑白图", WINDOW_KEEPRATIO);
		cv::imshow("精极耳黑白图", roi_er_good);
	}

	//精极耳截图  ***这里因为懒没有做越界检测 
	cv::Rect cut_er;
	cut_er.x = er_rec_good.x * roi_resizex;
	cut_er.width = er_rec_good.width * roi_resizex;
	cut_er.y = er_rec_good.y * roi_resizey;
	cut_er.height = er_rec_good.height * roi_resizey;

	jier = souce(cut_er);

	if (display)
	{
		cv::namedWindow("精极耳截图", WINDOW_KEEPRATIO);
		cv::imshow("精极耳截图", jier);
	}


	//本体ROI
	cv::Mat roi_bt;
	roi_bt = th_all - roi_er;

	if (display)
	{
		cv::namedWindow("本体黑白图", WINDOW_KEEPRATIO);
		cv::imshow("本体黑白图", roi_bt);
	}




	//本体截图  ***这里因为懒没有做越界检测 
	findContours(roi_bt, contours_bt, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
	sort(contours_bt.begin(), contours_bt.end(), size_max);  //1ms
	cv::Rect bt_rec = boundingRect(contours_bt[0]);

	bt_rec.x = (bt_rec.x - add_bt_x) > 0 ? (bt_rec.x - add_bt_x) : 0;
	bt_rec.width = ((bt_rec.x + bt_rec.width) + 2 * add_bt_x) < roi_er.cols ? (bt_rec.width + 2 * add_bt_x) : (roi_er.cols - bt_rec.x);
	bt_rec.y = (bt_rec.y - add_bt_y) > 0 ? (bt_rec.y - add_bt_y) : 0;
	bt_rec.height = ((bt_rec.y + bt_rec.height) + 2 * add_bt_y) < roi_er.rows ? (bt_rec.height + 2 * add_bt_y) : (roi_er.rows - bt_rec.y);
	rectangle(roi_er, bt_rec, Scalar(255, 255, 255), CV_FILLED, 8, 0);

	cv::Rect cut_bt;
	cut_bt.x = bt_rec.x * roi_resizex;
	cut_bt.width = bt_rec.width * roi_resizex;
	cut_bt.y = bt_rec.y * roi_resizey;
	cut_bt.height = bt_rec.height * roi_resizey;

	benti = souce(cut_bt);


	if (display)
	{
		cv::namedWindow("本体截图", WINDOW_KEEPRATIO);
		cv::imshow("本体截图", benti);
	}

    //用于显示的本体和极耳图
	cvtColor(souce, resultimage, COLOR_GRAY2BGR);
	resultbenti = resultimage(cut_bt);
	resultjier = resultimage(cut_er);



}

void Useopencv::Defect_defect(const bool& isbt,cv::Mat& cut, const cv::Mat& black, const Aloinputdata& inputdata,
	int& unknowpoint, int& errpoint, double& max_area, double& max_width, double& max_height)
{
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(black, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));

	unknowpoint = 0;
	errpoint = 0;
	max_area = 0;
	max_width = 0;
	max_height = 0;


	double resizex = double(cut.cols) / double(black.cols);
	double resizey = double(cut.rows) / double(black.rows);

	std::vector<Rect> allrect(contours.size());
	std::vector<int> all_width(contours.size());
	std::vector<int> all_height(contours.size());
	std::vector<int> all_area(contours.size());

#pragma omp parallel for

	for (int i = 0; i < contours.size(); i++)
	{
		//	printf("i = %d, I am Thread %d\n", i, omp_get_thread_num());
		allrect[i] = cv::boundingRect(contours[i]);

		int area = contours[i].size() * resizex * resizey;
		allrect[i].x *= resizex;
		allrect[i].y *= resizey;
		allrect[i].width *= resizex;
		allrect[i].height *= resizey;

		all_width[i] = allrect[i].width;
		all_height[i] = allrect[i].height;
		all_area[i] = area;


		/*	tempzzz[i] = contours[i].size();
			tempzzzz[i] = allrec[i].width;
			tempzzzzz[i] = allrec[i].height;*/


		if (display)
		{
			std::cout << "这是我后处理的面积          " << area * inputdata.scalex * inputdata.scaley << std::endl;
			std::cout << "这是我后处理的chang         " << allrect[i].width * inputdata.scalex << std::endl;
			std::cout << "这是我后处理的kuan          " << allrect[i].height * inputdata.scaley << std::endl;
		}

		int linesize = inputdata.aloparmetric[9] < 0 ? 5 : inputdata.aloparmetric[9];
		if (isbt)
		{
			if (allrect[i].width * inputdata.scalex < inputdata.aloparmetric[0] &&
				allrect[i].height * inputdata.scaley < inputdata.aloparmetric[1] &&
				all_area[i] * inputdata.scalex * inputdata.scaley < inputdata.aloparmetric[2])
			{
				cv::rectangle(cut, allrect[i], Scalar(0, 255, 0), linesize, LINE_8, 0);//绘制矩形
				unknowpoint++;
				//drawContours(rusultimage, contours, i, Scalar(0, 255, 255), 6 , 8 , hierarchy, 0, Point());
			}
			else
			{
				cv::rectangle(cut, allrect[i], Scalar(0, 0, 255), linesize, LINE_8, 0);//绘制矩形
				//drawContours(rusultimage, contours, i, Scalar(0, 0, 255), 6 , 8 , hierarchy, 0, Point());
				errpoint++;
			}

		}
		else
		{
			if (allrect[i].width * inputdata.scalex < inputdata.aloparmetric[3] &&
				allrect[i].height * inputdata.scaley < inputdata.aloparmetric[4] &&
				all_area[i] * inputdata.scalex * inputdata.scaley < inputdata.aloparmetric[5])
			{
				cv::rectangle(cut, allrect[i], Scalar(0, 255, 0), linesize, LINE_8, 0);//绘制矩形
				unknowpoint++;
				//drawContours(rusultimage, contours, i, Scalar(0, 255, 255), 6 , 8 , hierarchy, 0, Point());
			}
			else
			{
				cv::rectangle(cut, allrect[i], Scalar(0, 0, 255), linesize, LINE_8, 0);//绘制矩形
				//drawContours(rusultimage, contours, i, Scalar(0, 0, 255), 6 , 8 , hierarchy, 0, Point());
				errpoint++;
			}
		}


	}



	if (unknowpoint > 0 || errpoint > 0)
	{
		sort(all_width.begin(), all_width.end(), int_max);  //1ms
		sort(all_height.begin(), all_height.end(), int_max);  //1ms
		sort(all_area.begin(), all_area.end(), int_max);  //1ms
		max_width = double(all_width[0]) * inputdata.scalex;
		max_height = double(all_height[0]) * inputdata.scaley;
		max_area = double(all_area[0]) * inputdata.scalex * inputdata.scaley;
	}

}




