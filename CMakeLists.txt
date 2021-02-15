cmake_minimum_required(VERSION 3.10)
project(sfm LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE "Release")

add_subdirectory(3rdparty)

add_subdirectory(core)
# add_subdirectory(app)
# add_subdirectory(visualizer)
