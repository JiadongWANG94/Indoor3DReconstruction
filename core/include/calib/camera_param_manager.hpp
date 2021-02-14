#pragma once

#include <vector>
#include <string>

#include <Eigen/dense>
#include <opencv2/opencv.hpp>

#include "data_type/base.hpp"
#include "data_type/status.hpp"

namespace sfm {
namespace core {

class CameraParamManager {
 public:
    sfmStatus_t Init(const std::string &);
    sfmStatus_t Reset(const std::string &);

    sfmStatus_t GetK(Eigen::Matrix3d *K_mat) {
        if (nullptr != K_mat) {
            *K_mat = K_matrix_;
            return SFM_SUCCESS;
        } else {
            return SFM_NULL_PTR;
        }
    }

    sfmStatus_t GetDistortionCoefs(std::vector<float32_t> *dist_coefs) {
        if (nullptr != dist_coefs) {
            *dist_coefs = dist_coefs_;
            return SFM_SUCCESS;
        } else {
            return SFM_NULL_PTR;
        }
    }

 private:
    Eigen::Matrix3d K_matrix_;
    std::vecotr<float32_t> dist_coefs_;

};

}  // namespace core
}  // namespace sfm