#include "Useme.h"
#include "TensorRunnerNativeInterface.h"
#include "TFUtils.h"
#include <chrono>
//static img_process img_APP;


//TensorRunnerNativeInterface::TensorRunnerNativeInterface(const std::string& model_path, const std::vector<std::string>& pb_paths) :
//	utils(std::make_shared<TFUtils>()),
//	inception_utils(std::make_shared<TFUtils>()),
//	pb_paths(pb_paths)
//{
//	TFUtils::STATUS status = utils->LoadModel(model_path);
//	if (status != TFUtils::SUCCESS) {
//		utils.reset();
//		return;
//	}
//
//	status = inception_utils->LoadModel(pb_paths[0]);
//	if (status != TFUtils::SUCCESS) {
//		utils.reset();
//		return;
//	}
//
//	auto layers_name = inception_utils->GetAllOperationName();
//
//	auto ops = TF_NewSessionOptions();
//	//std::string str("allow_growth : True");
//	
//	uint8_t config[15] = { 0x32, 0xb, 0x9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x1, 0x38, 0x1 };
//	auto tfstatus = TF_NewStatus();
//
//
//
//	//TF_SetConfig(ops, (void*)config, 15, tfstatus);
//	//if (TF_GetCode(tfstatus) != TF_OK)
//	//{
//	//	std::cerr << "Edit allow_growth fail." << std::endl;
//	//}
//	TF_DeleteStatus(tfstatus);
//}

TensorRunnerNativeInterface::TensorRunnerNativeInterface(const std::string& model_path) :
	utils(std::make_shared<TFUtils>())
{
	TFUtils::STATUS status = utils->LoadModel(model_path);
	if (status != TFUtils::SUCCESS) {
		utils.reset();
		return;
	}
}


//
//std::vector<int> TensorRunnerNativeInterface::runInceptionAndUnet(cv::Mat img)
//{
//	if (!utils)
//	{
//		throw std::exception("Init fail.");
//
//	}
//	if (img.channels() != 1)
//	{
//		cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
//	}
//	cv::Mat bigroi;
//
//
//	auto getBIG_ROIstart_time = std::chrono::system_clock::now();
//	auto rtn = img_APP.getBIG_ROI(img, bigroi);
//	auto getBIG_ROIend_time = std::chrono::system_clock::now();
//
//	if (rtn < 0)
//	{
//		throw std::exception(("img_APP.getBIG_ROI err: return code " + std::to_string(rtn)).c_str());
//	}
//	auto dims = utils->GetOpsDimsByname("input_1", 0);
//
//	cv::resize(bigroi, bigroi, cv::Size(dims[1], dims[2]));
//
//	const std::vector<std::int64_t> input_dims = { 1, img.size().height, img.size().width, img.channels() };
//
//	TF_Tensor* input_image = Mat2Tensor(bigroi, 1 / 255.0);
//	size_t count = 0;
//	
//	// Input Tensor/Ops Create
//	const std::vector<TF_Tensor*> input_tensors = { input_image };
//
//	const std::vector<TF_Output> input_ops = { utils->GetOperationByName("input_1", 0) };
//
//	// Output Tensor/Ops Create
//	const std::vector<TF_Output> output_ops = { utils->GetOperationByName("output_0", 0) };
//
//	std::vector<TF_Tensor*> output_tensors = { nullptr };
//	auto unetstart_time = std::chrono::system_clock::now();
//	TFUtils::STATUS status = utils->RunSession(input_ops, input_tensors,
//		output_ops, output_tensors);
//	auto unetend_time = std::chrono::system_clock::now();
//
//	if (status != TFUtils::SUCCESS)
//	{
//		TFUtils::PrinStatus(status);
//		throw std::exception("Error run session");
//	}
//	auto rtn_data = TFUtils::GetTensorsDataMat<float>(output_tensors,dims);
//	
//	std::vector<cv::Mat> return_four_mini_roi;
//	cv::Mat unet_rtn_mat;
//	rtn_data[0].convertTo(unet_rtn_mat, CV_8UC1, 255.);
//
//	auto getFOUR_mini_ROIstart_time = std::chrono::system_clock::now();
//	img_APP.getFOUR_mini_ROI(bigroi, unet_rtn_mat, return_four_mini_roi);
//	auto getFOUR_mini_ROIend_time = std::chrono::system_clock::now();
//
//	std::vector<int> label_out;
//
//	auto inceptionstart_time = std::chrono::system_clock::now();
//	label_out.push_back(runInception(inception_utils, return_four_mini_roi[0]));
//	auto inceptionend_time = std::chrono::system_clock::now();
//
//	std::cout << "bigroi 运行时间：" << std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(getBIG_ROIstart_time - getBIG_ROIend_time).count()) << "ms" << std::endl;
//	std::cout << "getFOUR_mini_ROI 运行时间：" << std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(getFOUR_mini_ROIstart_time - getFOUR_mini_ROIend_time).count()) << "ms" << std::endl;
//	std::cout << "unet 运行时间：" << std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(unetstart_time - unetend_time).count()) << "ms" << std::endl;
//	std::cout << "inceptionv3 运行时间：" << std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(inceptionstart_time - inceptionend_time).count()) << "ms" << std::endl;
//
//	return label_out;
//}

std::vector<cv::Mat> TensorRunnerNativeInterface::runUnet(cv::Mat img)
{

	if (!utils)
	{
		throw std::exception("Init fail.");
	}
	if (img.channels() != 1)
	{
		cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);   //131
	}
	cv::Mat input_img;
	auto dims = utils->GetOpsDimsByname("input_1", 0);
	cv::resize(img, input_img, cv::Size(dims[1], dims[2]));

	//const std::vector<std::int64_t> input_dims = { 1, input_img.size().height, input_img.size().width, input_img.channels() };
	TF_Tensor* input_image = Mat2Tensor(input_img, 1 / 255.0);
	//size_t count = 0;

	// Input Tensor/Ops Create
	const std::vector<TF_Tensor*> input_tensors = { input_image };

	const std::vector<TF_Output> input_ops = { utils->GetOperationByName("input_1", 0) };

	// Output Tensor/Ops Create
	const std::vector<TF_Output> output_ops = { utils->GetOperationByName("output_0", 0) };

	std::vector<TF_Tensor*> output_tensors = { nullptr };

	//推理
	//auto unetstart_time = std::chrono::system_clock::now();
	TFUtils::STATUS status = utils->RunSession(input_ops, input_tensors,
		output_ops, output_tensors);

	if (status != TFUtils::SUCCESS)
	{
		TFUtils::PrinStatus(status);
		throw std::exception("Error run session");
	}

	//生成结果
	auto rtn_data = TFUtils::GetTensorsDataMat<float>(output_tensors, dims);

	//转换为0-255的cv::Mat
	for (cv::Mat & zzz : rtn_data)
	{
		cv::normalize(zzz, zzz, 255.0, 0.0, cv::NORM_MINMAX);
		zzz.cv::Mat::convertTo(zzz, CV_8UC1);
		cv::threshold(zzz, zzz, 0, 255, cv::THRESH_OTSU);
	}
	
	for (auto &i : input_tensors)
	{
		TF_DeleteTensor(i);
	}

	for (auto& i : output_tensors)
	{
		TF_DeleteTensor(i);
	}

	return rtn_data;
}

//std::vector<std::string> TensorRunnerNativeInterface::getLayersName()
//{
//	std::vector<std::string> out;
//	if (utils)
//	{
//		out = utils->GetAllOperationName();
//	}
//	return out;
//}
//
//int TensorRunnerNativeInterface::runInception(std::shared_ptr<TFUtils> iutils,cv::Mat source_img)
//{
//	cv::Mat img = source_img.clone();
//	const std::vector<TF_Output> inception_input_ops = { iutils->GetOperationByName("input", 0) };
//
//	// Output Tensor/Ops Create
//	const std::vector<TF_Output> inception_output_ops = { iutils->GetOperationByName("InceptionV3/Logits/SpatialSqueeze", 0) };
//
//
//	std::vector<TF_Tensor*> inception_output_tensors = { nullptr };
//
//
//	auto inception_dims = inception_utils->GetOpsDimsByname("input", 0);
//
//	cv::resize(img, img, cv::Size(inception_dims[1], inception_dims[2]));
//
//	cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
//
//	TF_Tensor* inception_input_image = Mat2Tensor(img, 1.);
//
//	const std::vector<TF_Tensor*> inception_input_tensors = { inception_input_image };
//
//	auto inception_out_dims = inception_utils->GetOpsDimsByname("InceptionV3/Logits/SpatialSqueeze", 0);
//
//
//	auto status = inception_utils->RunSession(inception_input_ops, inception_input_tensors, inception_output_ops, inception_output_tensors);
//	if (status != TFUtils::SUCCESS)
//	{
//		TFUtils::PrinStatus(status);
//		throw std::exception("Error run session");
//	}
//	return TFUtils::GetInceptionTensorsDataMat<float>(inception_output_tensors, inception_out_dims);
//}
