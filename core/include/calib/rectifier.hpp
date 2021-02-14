#pragma once

#include <vector>
#include <string>

#include <Eigen/dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core/cuda.hpp>

#include "data_type/base.hpp"
#include "data_type/status.hpp"

namespace sfm {
namespace core {

class Rectifier {
 public:
    sfmStatus_t Init(const Eigen::Matrix3d &K_mat, const std::vector<float32_t> &dist_coef);
    sfmStatus_t Init(const std::string &);

    sfmStatus_t Reset(const Eigen::Matrix3d &K_mat, const std::vector<float32_t> &dist_coef);
    sfmStatus_t Reset(const std::string &);

    sfmStatus_t Undistort(cv::Mat::Mat *image);
    sfmStatus_t Undistort(cv::cuda::GpuMat *image);

 private:
    Eigen::Matrix3d K_matrix_;
    std::vecotr<float32_t> dist_coefs_;

};

}  // namespace core
}  // namespace sfm