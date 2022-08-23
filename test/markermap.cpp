#include <aruco/aruco.h>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

#include <vector>
#include <iostream>

#include "markermap.h"

MarkerMap::MarkerMap(cv::Mat camMatrix, cv::Mat distCoeffs, cv::Size size)
{
    camera.setParams(camMatrix, distCoeffs, size);
    mmap.readFromFile("../board_configuration_0.035.yml");
    MMTracker.setParams(camera, mmap, 0.035);
    Detector.setDictionary(mmap.getDictionary()); // ARUCO_MIP_36h12

    mmap.getDictionary()
    ARUCO_MIP_36h12
}

void MarkerMap::detect(cv::Mat image)
{
    std::vector<aruco::Marker> markers = Detector.detect(image);

    std::vector<int> markers_from_set = mmap.getIndices(markers);
    for (auto idx : markers_from_set)
        markers[idx].draw(image, cv::Scalar(0, 0, 255), 2);
    if (MMTracker.estimatePose(markers))
    {
        std::cout << "RVec: " << MMTracker.getRvec() * 57.2958 << std::endl;

        // +X is Right on the screen, +Y is Up, +Z is INTO the screen
        aruco::CvDrawingUtils::draw3dAxis(image, camera, MMTracker.getRvec(), MMTracker.getTvec(), mmap[0].getMarkerSize() * 2);
    }
    cv::imshow("markermap", image);
    char key = (char)cv::waitKey(1);
}
