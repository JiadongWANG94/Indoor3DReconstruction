# VSfM

**WIP**
## Introduction
VSfM is a C++ project aimed to realize indoor reconstruction from a single RGB-Camera.
It can be run on a Jetson Nano board, or on X86-64 linux PC.

## Dependencies
- OpenCV
- Eigen (as submodule in `3rdparty`)
- CUDA

## Download and Build
```sh
mkdir build && cd build
cmake .. && make
```

## Validation Dataset
[Middlebury Temple](https://vision.middlebury.edu/mview/data/)
