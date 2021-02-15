#pragma once

#include <vector>
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>
#include <Eigen/Geometry>
#include <Eigen/Dense>

#include "data_type/base.hpp"

namespace sfm {
namespace core {

template <typename ImageType, typename FeatureType>
class Frame {
 public:
    std::shared_ptr<ImageType> &image() { return image_; }
    std::vector<Eigen::Quaternion<float32_t> > &camera_pose() { return camera_pose_; }
    std::vector<std::shared_ptr<FeatureType> > &features() { return features_; }
    std::vector<Eigen::Vector3f> &features_position() { return features_position_; }

 private:
    std::shared_ptr<ImageType> image_;
    std::vector<Eigen::Quaternion<float32_t> > camera_pose_;
    std::vector<std::shared_ptr<FeatureType> > features_;
    std::vector<Eigen::Vector3f> features_position_;
};

}
}