#include "feature_detector.hpp"

namespace sfm {
namespace core {

sfmStatus_t FeatureDetector::Detect(const cv::Mat::Mat &image, std::vector<std::shared_ptr<SURFFeature> > *features) {
    Ptr<Feature2D> surf = xfeatures2d::SURF::create(0, 3, 0.04, 10);
	std::vector<cv::KeyPoint> key_points;
	cv::Mat descriptors;
	sift->detectAndCompute(image, noArray(), key_points, descriptors);
    for (size_t i = 0; i < key_points.size(); ++ i) {
        cv::Mat descriptor(1, descriptors.rows, descriptors.type());
        descriptors.row(i).copyTo(descriptor);
        Eigen::Vector3d color;
        cv::Point2f &p = key_points[i].pt;
        if (p.x <= image.rows && p.y <= image.cols) {
            auto color_cv = image.at<Vec3b>(p.x, p.y);
            color.x() = color_cv.x;
            color.y() = color_cv.y;
            color.z() = color_cv.z;
        }
        feature->emplace_back(new SURFFeature(key_points[i], descriptor, color));
    }
}

}  // namespace core
}  // namespace sfm
