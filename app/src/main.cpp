
#include <vector>
#include <string>
#include <memory>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

#include "reconstruction/reconstructor.hpp"
#include "reconstruction/feature_detector.hpp"
#include "data_type/feature.hpp"

using namespace sfm::core;

int main() {

    std::vector<std::string> images{"./data/templeSR0001.png", "./data/templeSR0002.png"};
    Eigen::Matrix3f K;
    K << 1520.400000, 0.000000, 302.320000,
         0.000000, 1525.900000, 246.870000,
         0.000000, 0.000000, 1.000000;
    
    Reconstructor<cv::Mat, SURFFeature> reconstructor;

    std::shared_ptr<Frame<cv::Mat, SURFFeature> > first_frame = std::make_shared<Frame<cv::Mat, SURFFeature> >();
    std::shared_ptr<Frame<cv::Mat, SURFFeature> > second_frame = std::make_shared<Frame<cv::Mat, SURFFeature> >();

    cv::Mat image;
    image = cv::imread("./data/templeSparseRing/templeSR0001.png");
    std::shared_ptr<cv::Mat> first_image = std::make_shared<cv::Mat>();
    std::shared_ptr<cv::Mat> second_image = std::make_shared<cv::Mat>();
    image.copyTo(*first_image);
    image = cv::imread("./data/templeSparseRing/templeSR0002.png");
    image.copyTo(*second_image);

    std::cout << "image status dim : " << image.rows << " " << image.cols << std::endl;
    std::cout << "frame image status dim : " << first_image->rows << " " << first_image->cols << std::endl;

    first_frame->image() = first_image;
    second_frame->image() = second_image;

    FeatureDetector feature_detector;
    feature_detector.Detect(*(first_frame->image()), &(first_frame->features()));
    feature_detector.Detect(*(second_frame->image()), &(second_frame->features()));

    reconstructor.Init(K, first_frame);
    reconstructor.AddFrame(second_frame);

    const Structure &result = reconstructor.GetResults();

    std::ofstream outfile("pointcloud.ply");
    outfile << "ply\n" << "format ascii 1.0\n" << "comment SFM reconstruction points\n";
    outfile << "element vertex " << result.points.size() << "\n";
    outfile << "property float x\n" << "property float y\n" << "property float z\n" << "element face 0\n";
    outfile << "property list uchar int vertex_indices\n" << "end_header\n";
    for (int i = 0; i < result.points.size(); i++)
    {
        const Eigen::Vector3f &point = result.points.at(i);
        outfile << point.x() << " ";
        outfile << point.y() << " ";
        outfile << point.z() << " ";
        outfile << "\n";
    }
    outfile.close();

    return 0;
}