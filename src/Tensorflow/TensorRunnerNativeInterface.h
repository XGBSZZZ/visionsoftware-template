#pragma once

#include "Useme.h"


#include <string>
#include <memory>
#include "opencv2/opencv.hpp"

#include "TFUtils.h"
#include <opencv2/imgproc/types_c.h>



class  TensorRunnerNativeInterface
{
public:
	TensorRunnerNativeInterface(const std::string& model_path);
	std::vector<cv::Mat> runUnet(cv::Mat img);
	//TensorRunnerNativeInterface(const std::string& model_path, const std::vector<std::string>& pb_paths);
	//std::vector<int> runInceptionAndUnet(cv::Mat img);
	//std::vector<std::string> getLayersName();

private:
	std::shared_ptr<TFUtils> utils;
	TF_Tensor* Mat2Tensor(cv::Mat& img, float normal = 1 / 255.0) {
		const std::vector<std::int64_t> input_dims = { 1, img.size().height, img.size().width, img.channels() };

		// Convert to float 32 and do normalize ops
		cv::Mat fake_mat(img.rows, img.cols, CV_32FC(img.channels()));
		img.convertTo(fake_mat, CV_32FC(img.channels()), normal);
		//fake_mat *= normal;

		TF_Tensor* image_input = TFUtils::CreateTensor(TF_FLOAT,
			input_dims.data(), input_dims.size(),
			fake_mat.data, (fake_mat.size().height * fake_mat.size().width * fake_mat.channels() * sizeof(float)));

		return image_input;

	}
	//int runInception(std::shared_ptr<TFUtils> iutils, cv::Mat source_img);
	//std::vector<std::string> pb_paths;
	//std::shared_ptr<TFUtils> inception_utils;
};

