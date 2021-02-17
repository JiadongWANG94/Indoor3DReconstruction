
#include "reconstruction/reconstructor.hpp"

#include <opencv2/core/eigen.hpp>

#include "reconstruction/matcher.hpp"
#include "data_type/feature.hpp"

namespace sfm {
namespace core {

template <>
sfmStatus_t Reconstructor<cv::Mat, SURFFeature>::AddFrame(
    std::shared_ptr<Frame<cv::Mat, SURFFeature> > &frame) {

    std::vector<SURFFeature> src_features, dest_features;
    for (auto &p_feature : base_frame_->features()) {
        src_features.push_back(*p_feature);
    }
    std::cout << "Reconstructor : size of src features is " << base_frame_->features().size() << std::endl;
    for (auto &p_feature : frame->features()) {
        dest_features.push_back(*p_feature);
    }
    std::cout << "Reconstructor : size of dest features is " << frame->features().size() << std::endl;
    std::vector<std::pair<size_t, size_t> > matched_pairs;
    Matcher<SURFFeature>().CalculateMatchedPairs(src_features, dest_features, &matched_pairs);
    std::cout << "Reconstructor :  size of matched pairs is " << matched_pairs.size() << std::endl;
    
    std::vector<cv::Point2f> src_point_list, dest_point_list;
    for (size_t i = 0; i < matched_pairs.size(); ++i) {
        Eigen::Vector2f position;
        src_features.at(i).GetPosition(&position);
        src_point_list.emplace_back(position.x(),position.y());
        dest_features.at(i).GetPosition(&position);
        dest_point_list.emplace_back(position.x(),position.y());
    }

    cv::Mat mask;
    cv::Mat E = findEssentialMat(src_point_list, dest_point_list, focal_length_, principle_point_, cv::RANSAC, 0.999, 1.0, mask);
    if (E.empty()) {
        std::cout << "Reconstructor : Failed to find essential matrix." << std::endl;
        return SFM_INTERNAL_ERROR;
    }

    float64_t feasible_count = countNonZero(mask);

    if (feasible_count <= 15 || (feasible_count / src_point_list.size()) < 0.6) {
        std::cout << "Reconstructor : Invalid results." << std::endl;
        std::cout << "Reconstructor : EssentialMat : " << E << std::endl;
        std::cout << "Reconstructor : mask : " << mask << std::endl;
        return SFM_INTERNAL_ERROR;
    }

    cv::Mat R, T;
    int pass_count = recoverPose(E, src_point_list, dest_point_list, R, T, focal_length_, principle_point_, mask);

    cv::Mat proj1(3, 4, CV_32FC1);
    cv::Mat proj2(3, 4, CV_32FC1);

    proj1(cv::Range(0, 3), cv::Range(0, 3)) = cv::Mat::eye(3, 3, CV_32FC1);
    proj1.col(3) = cv::Mat::zeros(3, 1, CV_32FC1);

    R.convertTo(proj2(cv::Range(0, 3), cv::Range(0, 3)), CV_32FC1);
    T.convertTo(proj2.col(3), CV_32FC1);

    cv::Mat K;
    cv::eigen2cv(K_, K);
    cv::Mat fK;
    K.convertTo(fK, CV_32FC1);
    proj1 = fK * proj1;
    proj2 = fK * proj2;

    cv::Mat triangulated_points_homogeneous;
    cv::triangulatePoints(proj1, proj2, src_point_list, dest_point_list, triangulated_points_homogeneous);
    // std::cout << "Reconstructor : Results Homogeneous : " << std::endl << triangulated_points_homogeneous << std::endl;

    cv::Mat triangulated_points_3d;
    cv::transpose(triangulated_points_homogeneous, triangulated_points_3d);
    cv::convertPointsFromHomogeneous(triangulated_points_3d, triangulated_points_3d);
    std::cout << "Reconstructor : Results 3D : " << std::endl << triangulated_points_3d << std::endl;

    for (int idx = 0; idx < triangulated_points_3d.rows; ++idx) {
        structure_.points.emplace_back(
            triangulated_points_3d.at<float32_t>(idx, 0),
            triangulated_points_3d.at<float32_t>(idx, 1),
            triangulated_points_3d.at<float32_t>(idx, 2));
    }

    std::cout << "Reconstructor : Succeed." << std::endl;
}

}  // namespace core
}  // namespace sfm
