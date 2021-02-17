#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <opencv2/core/base.hpp>

#include "data_type/status.hpp"
#include "data_type/feature.hpp"

namespace sfm {
namespace core {

template <typename T> struct is_shared_ptr : std::false_type {};

template <typename T> struct is_shared_ptr<std::shared_ptr<T> > : std::true_type {};

template <bool, typename TypeIfTrue, typename TypeIfFalse>
struct enable_if_else {};

template <typename TypeIfTrue, typename TypeIfFalse>
struct enable_if_else<false, TypeIfTrue, TypeIfFalse> {
    typedef TypeIfFalse type;
};

template <typename TypeIfTrue, typename TypeIfFalse>
struct enable_if_else<true, TypeIfTrue, TypeIfFalse> {
    typedef TypeIfTrue type;
};

template <typename T>
class Matcher {
 private:
    class DistNonPtr {
     public:
        float32_t operator()(const T &first, const T &second) { return first.ComputeDistance(second); }
    };
    class DistPtr {
     public:
        float32_t operator()(const T &first, const T &second) { return first->ComputeDistance(*second); }
    };
    using DistBetween = typename enable_if_else<is_shared_ptr<T>::value, DistPtr, DistNonPtr>::type;
 public: 
    sfmStatus_t CalculateMatchedPairs(
        const std::vector<T> &src_features,
        const std::vector<T> &dest_features,
        std::vector<std::pair<size_t, size_t> > *pairs) {
        // TODO
        std::cout << "Not implemented." << std::endl;
        return SFM_INTERNAL_ERROR;
    }
};

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
    std::cout << "Matcher : src_mat dim : " << src_mat.rows << " " << src_mat.cols << std::endl;
    // std::cout << "src_mat "  << src_mat << std::endl;
    cv::vconcat(dest_features[0].GetDescriptor(), dest_features[1].GetDescriptor(), dest_mat);
    for (auto ite = dest_features.begin() + 2; ite != dest_features.end(); ++ ite) {
        cv::vconcat(dest_mat, ite->GetDescriptor(), dest_mat);
    }
    std::cout << "Matcher : dest_mat dim : " << dest_mat.rows << " " << dest_mat.cols << std::endl;
    // std::cout << "dest_mat "  << dest_mat << std::endl;
    matcher.knnMatch(src_mat, dest_mat, knn_matches, 5);
	float min_dist = FLT_MAX;
    std::cout << "Matcher : knn_matches size : " << knn_matches.size() << std::endl;
    for (int r = 0; r < knn_matches.size(); ++r) {
        // Rotio Test
        // if (knn_matches[r][0].distance > 0.6 * knn_matches[r][1].distance) {
        //     continue;
        // }

        float dist = knn_matches[r][0].distance;
        if (dist < min_dist) {
            min_dist = dist;
        }
    }

    for (size_t r = 0; r < knn_matches.size(); ++r) {
        // 排除不满足Ratio Test的点和匹配距离过大的点
        // if (
        //     knn_matches[r][0].distance > 0.6 * knn_matches[r][1].distance ||
        //     knn_matches[r][0].distance > 5 * std::max(min_dist, 10.0f)
        //     ) {
        //     continue;
        // }

        // 保存匹配点
        pairs->push_back(std::make_pair<size_t, size_t>(knn_matches[r][0].queryIdx, knn_matches[r][0].trainIdx));
    }
    return SFM_SUCCESS;
}


}  // namespace core
}  // namespace sfm