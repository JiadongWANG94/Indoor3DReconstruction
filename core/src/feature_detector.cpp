#include "reconstruction/feature_detector.hpp"

#include <iostream>
#include <opencv2/features2d.hpp>

namespace sfm {
namespace core {

sfmStatus_t FeatureDetector::Detect(const cv::Mat &image, std::vector<std::shared_ptr<SURFFeature> > *features) {
    cv::Ptr<cv::Feature2D> orb = cv::ORB::create();
	std::vector<cv::KeyPoint> key_points;
	cv::Mat descriptors;
	orb->detectAndCompute(image, cv::noArray(), key_points, descriptors);
    std::cout << "FeatureDetector : size of feature detected is " << key_points.size() << std::endl;
    std::cout << "Image status : " << std::ends << " dim : " << image.rows << " " << image.cols << std::endl;
    for (size_t i = 0; i < key_points.size(); ++ i) {
        cv::Mat descriptor(1, descriptors.rows, descriptors.type());
        descriptors.row(i).copyTo(descriptor);
        Eigen::Vector3d color;
        cv::Point2f &p = key_points[i].pt;
        if ((int)p.x <= image.rows && (int)p.y <= image.cols) {
            auto color_cv = image.at<cv::Vec3b>((int)p.x, (int)p.y);
            color.x() = color_cv[0];
            color.y() = color_cv[1];
            color.z() = color_cv[2];
        }
        features->emplace_back(new SURFFeature(key_points[i], descriptor, color));
    }
}

}  // namespace core
}  // namespace sfm
