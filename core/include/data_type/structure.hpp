#pragma once

#include <vector>
#include <Eigen/Dense>

namespace sfm {
namespace core {
struct Structure {
    std::vector<Eigen::Vector3f> points;
    std::vector<Eigen::Vector3d> colors;
};

}  // namespace core
}  // namespace sfm