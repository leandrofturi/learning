#pragma once

#include <opencv2/opencv.hpp>
#include <aruco/aruco.h>

class MarkerMap
{
public:
    aruco::CameraParameters camera;
    aruco::MarkerMap mmap;
    aruco::MarkerMapPoseTracker MMTracker;
    aruco::MarkerDetector Detector;

    MarkerMap(cv::Mat camMatrix, cv::Mat distCoeffs, cv::Size size);

    void detect(cv::Mat image);
};
