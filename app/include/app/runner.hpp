#pragma once

#include <vector>
#include <string>

#include "data_type/status.hpp"
#include "data_type/image.hpp"

namespace sfm {
namespace app {
class Executor {
 public:
    sfmStatus_t Init();
    sfmStatus_t Process(std::vector<sfm::core::Image> images);
    sfmStatus_t Exit();
 private:

};

}  // namespace app
}  // namespace sfm
