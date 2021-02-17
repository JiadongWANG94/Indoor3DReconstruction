#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <Eigen/Dense>

#include "data_type/status.hpp"
#include "data_type/base.hpp"

namespace sfm {
namespace core {

class Feature {
 public:
    virtual ~Feature() {}
    virtual float32_t ComputeDistance(const Feature &rhs) const = 0;
    virtual sfmStatus_t GetPosition(Eigen::Vector2f *position) const = 0;
    virtual sfmStatus_t GetColor(Eigen::Vector3d *color) const = 0;
};

class SURFFeature : public Feature {
 public:
    SURFFeature(const cv::KeyPoint &key_point, const cv::Mat &descriptor, const Eigen::Vector3d &color) :
        key_point_(key_point), descriptor_(descriptor), color_(color) {}

    sfmStatus_t GetPosition(Eigen::Vector2f *position) const override {
        if (nullptr == position) {
            return SFM_NULL_PTR;
        }
        position->x() = key_point_.pt.x;
        position->y() = key_point_.pt.y;
        return SFM_SUCCESS;
    }

    sfmStatus_t GetColor(Eigen::Vector3d *color) const override {
        *color = color_;
        return SFM_SUCCESS;
    }

    float32_t ComputeDistance(const Feature &rhs) const override {
        return 0;
    }

    const cv::Mat &GetDescriptor() const { return descriptor_; }

 private:
    cv::KeyPoint key_point_;
    cv::Mat descriptor_;
    Eigen::Vector3d color_;
};


}  // namespace core
}  // namespace sfm