#include "matcher.hpp"

#include <algorithm>
#include <opencv2/core/base.hpp>

namespace sfm {
namespace core {

template <>
sfmStatus_t Matcher<SURFFeature>::CalculateMatchedPairs(
    const std::vector<SURFFeature> &src_features,
    const std::vector<SURFFeature> &dest_features,
    std::vector<std::pair<size_t, size_t> > *pairs) {
    if (nullptr == pairs) {
        std::cout << "Matcher : nullptr" << std::endl;
        return SFM_NULL_PTR;
    }
    if (src_features.size() <= 2 || dest_features.size() <= 2) {
        std::cout << "Matcher : feature size too small" << std::endl;
        return SFM_INTERNAL_ERROR;
    }
    std::vector<std::vector<cv::DMatch>> knn_matches;
    cv::BFMatcher matcher(cv::NormTypes::NORM_L2);
    cv::Mat src_mat, dest_mat;
    cv::vconcat(src_features[0].GetDescriptor(), src_features[1].GetDescriptor(), src_mat);
    for (auto ite = src_features.begin() + 2; ite != src_features.end(); ++ ite) {
        cv::vconcat(src_mat, ite->GetDescriptor(), src_mat);
    }
    cv::vconcat(dest_features[0].GetDescriptor(), dest_features[1].GetDescriptor(), dest_mat);
    for (auto ite = dest_features.begin() + 2; ite != dest_features.end(); ++ ite) {
        cv::vconcat(dest_mat, ite->GetDescriptor(), dest_mat);
    }
    matcher.knnMatch(src_mat, dest_mat, knn_matches, 2);
	float min_dist = FLT_MAX;
    for (int r = 0; r < knn_matches.size(); ++r) {
        // Rotio Test
        if (knn_matches[r][0].distance > 0.6 * knn_matches[r][1].distance) {
            continue;
        }

        float dist = knn_matches[r][0].distance;
        if (dist < min_dist) {
            min_dist = dist;
        }
    }

    for (size_t r = 0; r < knn_matches.size(); ++r) {
        // 排除不满足Ratio Test的点和匹配距离过大的点
        if (
            knn_matches[r][0].distance > 0.6 * knn_matches[r][1].distance ||
            knn_matches[r][0].distance > 5 * std::max(min_dist, 10.0f)
            ) {
            continue;
        }

        // 保存匹配点
        pairs->push_back(std::make_pair<size_t, size_t>(knn_matches[r][0].queryIdx, knn_matches[r][0].trainIdx));
    }
    return SFM_SUCCESS;
}

}  // namespace core
}  // namespace sfm