
#pragma once

#include <vector>
#include <memory>

#include "data_type/status.hpp"
#include "data_type/frame.hpp"
#include "data_type/structure.hpp"
#include "data_type/base.hpp"

namespace sfm {
namespace core {

template <typename ImageType, typename FeatureType>
class Reconstructor {
 public:
    sfmStatus_t Init(
        const Eigen::Matrix3f &K,
        std::shared_ptr<Frame<ImageType, FeatureType> > &base_frame) {
        K_ = K;
        base_frame_= base_frame;
        principle_point_.x = K(0,3);
        principle_point_.y = K(1,3);
        focal_length_ = 0.5 * (K(0,0) + K(1,0));
    }
    
    sfmStatus_t AddFrame(std::shared_ptr<Frame<ImageType, FeatureType> > &frame);

    sfmStatus_t AddFrames(std::vector<std::shared_ptr<Frame<ImageType, FeatureType> > > &frames);

    sfmStatus_t Optimize();

    const Structure &GetResults() const { return structure_; };
 private:
    sfmStatus_t find_transform();
    sfmStatus_t reconstruct();
 private:
    Structure structure_;
    Eigen::Matrix3f K_;
    std::shared_ptr<Frame<ImageType, FeatureType> > base_frame_;
    cv::Point2d principle_point_;
    float64_t focal_length_;
};

}  // namespace core
}  // namespace sfm
