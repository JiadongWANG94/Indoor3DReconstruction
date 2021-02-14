
#include "reconstructor.hpp"

namespace sfm {
namespace core {


sfmStatus_t Reconstructor<cv::Mat, SURFFeature>::AddFrame(
    std::shared_ptr<Frame<cv::Mat, SURFFeature> > &frame) {

    std::vector<SURFFeature> src_features, dest_features;
    for (auto &p_feature : base_frame_.features()) {
        src_features.push_back(*p_features);
    }
    for (auto &p_feature : frame.features()) {
        dest_features.push_back(*p_features);
    }
    std::vector<std::pair<size_t, size_t> > matched_pairs;
    Matcher<SURFFeature>().CalculateMatchedPairs(src_features, dest_features, &matched_pairs);

    
    std::vector<cv::Point2f> src_point_list, dest_point_list;
    for (size_t i = 0; i < matched_points.size(); ++i) {
        Eigen::Vector2f position;
        src_features.at(i).GetPosition(&position);
        src_point_list.emplace_back(position.x(),position.y());
        dest_features.at(i).GetPosition(&position);
        dest_point_list.emplace_back(position.x(),position.y());
    }

    cv::Mat mask;
    cv::Mat E = findEssentialMat(src_point_list, dest_point_list, focal_length_, principle_point_, RANSAC, 0.999, 1.0, mask);
    if (E.empty()) {
        std::cout << "Reconstructor : Failed to find essential matrix." << std::endl;
        return SFM_INTERNAL_ERROR;
    }

    float64_t feasible_count = countNonZero(mask);

    if (feasible_count <= 15 || (feasible_count / p1.size()) < 0.6) {
        std::cout << "Reconstructor : Invalid results." << std::endl;
        return SFM_INTERNAL_ERROR;
    }

    cv::Mat R, T;
    int pass_count = recoverPose(E, src_point_list, dest_point_list, R, T, focal_length_, principle_point_, mask);

    cv::Mat proj1(3, 4, CV_32FC1);
    cv::Mat proj2(3, 4, CV_32FC1);

    proj1(Range(0, 3), Range(0, 3)) = cv::Mat::eye(3, 3, CV_32FC1);
    proj1.col(3) = cv::Mat::zeros(3, 1, CV_32FC1);

    R.convertTo(proj2(Range(0, 3), Range(0, 3)), CV_32FC1);
    T.convertTo(proj2.col(3), CV_32FC1);

    cv::Mat fK;
    K.convertTo(fK, CV_32FC1);
    proj1 = fK * proj1;
    proj2 = fK * proj2;

    cv::Mat structure;
    triangulatePoints(proj1, proj2, src_point_list, dest_point_list, structure);
}

}  // namespace core
}  // namespace sfm
