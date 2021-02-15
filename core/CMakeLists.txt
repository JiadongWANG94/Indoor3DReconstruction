cmake_minimum_required(VERSION 3.10)
project(core LANGUAGES C CXX)

###############
##  general  ##
###############

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_CXX_FLAGS "-fPIC")

set(lib_name ${PROJECT_NAME})

############
##  deps  ##
############

find_package(OpenCV REQUIRED
)

#############
##  build  ##
#############

file(GLOB_RECURSE cpp_src "./src/*.cpp")
file(GLOB_RECURSE cu_src "./src/*.cu")

add_library(${lib_name} SHARED)
target_sources(${lib_name}
    PRIVATE
        ${cpp_src}
)

target_include_directories(${lib_name}
    PUBLIC
        include
        ${OpenCV_INCLUDE_DIRS}
    PRIVATE
        src
)

target_link_directories(${lib_name}
    PUBLIC
        # ${OpenCV_LIB_DIRS}
)

target_link_libraries(${lib_name}
    PUBLIC
        ${OpenCV_LIBS}
        Eigen
)


