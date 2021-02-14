#pragma once

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <Eigen/Geometry>
#include <Eigen/Dense>

namespace sfm {
namespace core {

template <typename ImageType, typename FeatureType>
class Frame {
 public:
    std::shared_ptr<ImageType> image() { return image_; }
    std::vector<std::Eigen::Quaternion> camera_pose() { return camera_pose_; }
    std::vector<std::shared_ptr<FeatureType> > features() { return features_; }
    std::vector<std::Eigen::Vector3f> features_position() { return features_position_; }

 private:
    std::shared_ptr<ImageType> image_;
    std::vector<std::Eigen::Quaternion> camera_pose_;
    std::vector<std::shared_ptr<FeatureType> > features_;
    std::vector<std::Eigen::Vector3f> features_position_;
};

}
}