#pragma once
#include "Useme.h"

#if defined(_MSC_VER)
#  if !defined(COMPILER_MSVC)
#    define COMPILER_MSVC // Set MSVC visibility of exported symbols in the shared library.
#  endif
#  pragma warning(push)
#  pragma warning(disable : 4190)
#endif

#include "tensorflow/c/c_api.h"
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"

class TFUtils {
public:
    enum STATUS
    {
        SUCCESS = 0,
        SESSION_CREATE_FAILED = 1,
        MODEL_LOAD_FAILED = 2,
        FAILED_RUN_SESSION = 3,
        MODEL_NOT_LOADED = 4,
    };
    TFUtils();
    STATUS LoadModel(std::string model_file);
    ~TFUtils();

    TF_Output GetOperationByName(std::string name, int idx);
    std::vector<std::string> GetAllOperationName();
    STATUS RunSession(const std::vector<TF_Output>& inputs, const std::vector<TF_Tensor*>& input_tensors,
        const std::vector<TF_Output>& outputs, std::vector<TF_Tensor*>& output_tensors);

    // Static functions
    template <typename T>
    static TF_Tensor* CreateTensor(TF_DataType data_type,
        const std::vector<std::int64_t>& dims,
        const std::vector<T>& data) {
        return CreateTensor(data_type,
            dims.data(), dims.size(),
            data.data(), data.size() * sizeof(T));
    }

    std::vector<int64_t> GetOpsDimsByname(const std::string& name,int idx);


    static void DeleteTensor(TF_Tensor* tensor);

    static void DeleteTensors(const std::vector<TF_Tensor*>& tensors);

    template <typename T>
    static std::vector<std::vector<T>> GetTensorsData(const std::vector<TF_Tensor*>& tensors) {
        std::vector<std::vector<T>> data;
        data.reserve(tensors.size());
        for (const auto t : tensors) {
            data.push_back(GetTensorData<T>(t));
        }

        return data;
    }

    template <typename T>
    std::vector<cv::Mat> TF_GetTensorsDataMat(const std::vector<TF_Tensor*>& tensors, std::vector<int64_t> dims) {
        std::vector<cv::Mat> rtn_data;
        for (int i = 0; i < tensors.size(); i++)
        {
            cv::Mat data_mat(cv::Size(dims[1], dims[2]), CV_32FC(dims[3]));
            int tensor_size = TF_TensorByteSize(tensors[i]);
            auto data_ptr = static_cast<T*>(TF_TensorData(tensors[i]));
            int byte_count = 0;
            for (int r = 0; r < data_mat.rows; r++)
            {
                for (int c = 0; c < data_mat.cols; c++)
                {
                    data_mat.at<T>(r, c) = data_ptr[byte_count];
                    byte_count++;
                }
            }
            rtn_data.push_back(data_mat);
        }
        return rtn_data;
    }

    template <typename T>
    static std::vector<cv::Mat> GetTensorsDataMat(const std::vector<TF_Tensor*>& tensors,std::vector<int64_t> dims) {
        std::vector<cv::Mat> rtn_data;
        for (int i = 0; i < tensors.size(); i++)
        {
            cv::Mat data_mat(cv::Size(dims[1],dims[2]),CV_32FC(dims[3]));
            int tensor_size = TF_TensorByteSize(tensors[i]);
            auto data_ptr = static_cast<T*>(TF_TensorData(tensors[i]));
            int byte_count = 0;
            for (int r = 0; r < data_mat.rows; r++)
            {
                for (int c = 0; c < data_mat.cols; c++)
                {
                    data_mat.at<T>(r, c) = data_ptr[byte_count];
                    byte_count++;
                }
            }
            rtn_data.push_back(data_mat);
        }
        return rtn_data;
    }

    template <typename T>
    static int GetInceptionTensorsDataMat(const std::vector<TF_Tensor*>& tensors, std::vector<int64_t> dims) {

        cv::Mat data_mat(cv::Size(dims[0], dims[1]), CV_32FC1);
        int tensor_size = TF_TensorByteSize(tensors[0]);
        auto data_ptr = static_cast<T*>(TF_TensorData(tensors[0]));
        int byte_count = 0;
        for (int r = 0; r < data_mat.rows; r++)
        {
            for (int c = 0; c < data_mat.cols; c++)
            {
                data_mat.at<T>(r, c) = data_ptr[byte_count];
                byte_count++;
            }
        }
        cv::Point maxLoc;
        cv::minMaxLoc(data_mat,nullptr,nullptr,nullptr,&maxLoc);
        
        return maxLoc.y;
    }

    static TF_Tensor* CreateTensor(TF_DataType data_type,
        const std::int64_t* dims, std::size_t num_dims,
        const void* data, std::size_t len);

    template <typename T>
    static std::vector<T> GetTensorData(const TF_Tensor* tensor) {
        const auto data = static_cast<T*>(TF_TensorData(tensor));
        if (data == nullptr) {
            return {};
        }

        return { data, data + (TF_TensorByteSize(tensor) / TF_DataTypeSize(TF_TensorType(tensor))) };
    }
    //    STATUS GetErrorCode();
    static void PrinStatus(STATUS status);

private:
    TF_Graph* graph_def;
    TF_Session* sess;
    STATUS init_error_code;

private:
    TF_Graph* LoadGraphDef(const char* file);

    TF_Session* CreateSession(TF_Graph* graph);

    bool CloseAndDeleteSession(TF_Session* sess);

    bool RunSession(TF_Session* sess,
        const TF_Output* inputs, TF_Tensor* const* input_tensors, std::size_t ninputs,
        const TF_Output* outputs, TF_Tensor** output_tensors, std::size_t noutputs);

    bool RunSession(TF_Session* sess,
        const std::vector<TF_Output>& inputs, const std::vector<TF_Tensor*>& input_tensors,
        const std::vector<TF_Output>& outputs, std::vector<TF_Tensor*>& output_tensors);


}; // End class TFUtils

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif