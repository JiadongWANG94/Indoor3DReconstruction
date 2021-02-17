cmake_minimum_required(VERSION 3.10)
project(app LANGUAGES C CXX)

###############
##  general  ##
###############

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_CXX_FLAGS "-fPIC")

set(exe_name ${PROJECT_NAME})

############
##  deps  ##
############

#############
##  build  ##
#############

file(GLOB_RECURSE cpp_src "./src/*.cpp")
file(GLOB_RECURSE cu_src "./src/*.cu")

add_executable(${exe_name})
target_sources(${exe_name}
    PRIVATE
        ${cpp_src}
)

target_include_directories(${exe_name}
    PUBLIC
        include
        ${OpenCV_INCLUDE_DIRS}
    PRIVATE
        src
)

target_link_directories(${exe_name}
    PUBLIC
        # ${OpenCV_LIB_DIRS}
)

target_link_libraries(${exe_name}
    PUBLIC
        ${OpenCV_LIBS}
        Eigen
    PRIVATE
        core

)


