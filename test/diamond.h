#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/aruco/charuco.hpp>

class Diamond
{
public:
    cv::Mat camMatrix;
    cv::Mat distCoeffs;
    cv::Size size;

    cv::Ptr<cv::aruco::DetectorParameters> detectorParams;
    cv::Ptr<cv::aruco::Dictionary> dictionary;

    Diamond(cv::Mat _camMatrix, cv::Mat _distCoeffs, cv::Size _size);

    void detect(cv::Mat image);
};
