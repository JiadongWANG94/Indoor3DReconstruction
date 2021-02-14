#pragma once

#include <vector>
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <Eigen/Geometry>
#include <Eigen/Dense>

#include "data_type/base.hpp"
#include "data_type/status.hpp"
#include "data_type/feature.hpp"
#include "data_type/frame.hpp"

namespace sfm {
namespace core {

using OpencvFrame = Frame<cv::Mat::Mat, SURFFeature>;

class FeatureDetector {
 public:
    sfmStatus_t Detect(OpencvFrame &frame) {
        return this->Detect(*frame.image(), &frame.features());
    }

    sfmStatus_t Detect(const cv::Mat::Mat &image, std::vector<std::shared_ptr<SURFFeature> > *features);
};

}  // namespace core
}  // namespace sfm
