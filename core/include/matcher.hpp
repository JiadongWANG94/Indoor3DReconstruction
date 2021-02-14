#pragma once

#include <vector>
#include <memory>
#include <type_traits>

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
        std::vector<std::pair<size_t, size_t> > *pairs);
    
};

template<typename T>
sfmStatus_t Matcher<T>::CalculateMatchedPairs(
    const std::vector<T> &src_features,
    const std::vector<T> &dest_features,
    std::vector<std::pair<size_t, size_t> > *pairs) {
    // TODO

}

}  // namespace core
}  // namespace sfm