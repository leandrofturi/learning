#include <sl/Camera.hpp>
#include <aruco/aruco.h>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco/charuco.hpp>

#include <vector>
#include <iostream>


int main(int argc, char *argv[])
{
    sl::Camera zed;

    sl::InitParameters init_params;
    init_params.sdk_verbose = false;
    init_params.camera_resolution = sl::RESOLUTION::HD720;
    init_params.coordinate_units = sl::UNIT::METER;
    init_params.sensors_required = false;

    sl::ERROR_CODE err = zed.open(init_params);
    if (err != sl::ERROR_CODE::SUCCESS)
        exit(-1);

    // in pixels
    float fx = zed.getCameraInformation().calibration_parameters.left_cam.fx,
          fy = zed.getCameraInformation().calibration_parameters.left_cam.fy,
          cx = zed.getCameraInformation().calibration_parameters.left_cam.cx,
          cy = zed.getCameraInformation().calibration_parameters.left_cam.cy;

    double k1 = zed.getCameraInformation().calibration_parameters.left_cam.disto[0],
           k2 = zed.getCameraInformation().calibration_parameters.left_cam.disto[1],
           p1 = zed.getCameraInformation().calibration_parameters.left_cam.disto[2],
           p2 = zed.getCameraInformation().calibration_parameters.left_cam.disto[3],
           k3 = zed.getCameraInformation().calibration_parameters.left_cam.disto[4],
           k4 = zed.getCameraInformation().calibration_parameters.left_cam.disto[5],
           k5 = zed.getCameraInformation().calibration_parameters.left_cam.disto[6],
           k6 = zed.getCameraInformation().calibration_parameters.left_cam.disto[7];

    sl::Resolution image_size = zed.getCameraInformation().camera_resolution;
    sl::Mat image_zed = sl::Mat(image_size, sl::MAT_TYPE::U8_C4);
    cv::Mat image_ocv = cv::Mat(image_zed.getHeight(), image_zed.getWidth(), CV_8UC4, image_zed.getPtr<sl::uchar1>(sl::MEM::CPU));
    cv::Mat image;

    // https://docs.opencv.org/2.4/modules/imgproc/doc/geometric_transformations.html#undistort
    cv::Mat camMatrix = (cv::Mat_<double>(3, 3) << fx, 0., cx, 0., fy, cy, 0., 0., 1.);
    cv::Mat distCoeffs = (cv::Mat_<double>(5, 1) << 0., 0., 0., 0., 0.);
    cv::Size size(1280, 720);
    aruco::CameraParameters camera(camMatrix, distCoeffs, size);

    aruco::MarkerMap mmap;
    mmap.readFromFile("board_configuration_0.035.yml");

    aruco::MarkerMapPoseTracker MMTracker;
    MMTracker.setParams(camera, mmap, 0.035);
    aruco::MarkerDetector Detector;
    Detector.setDictionary(mmap.getDictionary()); // ARUCO_MIP_36h12

    while (zed.grab() == sl::ERROR_CODE::SUCCESS)
    {
        zed.retrieveImage(image_zed, sl::VIEW::LEFT, sl::MEM::CPU, image_size);
        cv::cvtColor(image_ocv, image, cv::COLOR_RGBA2RGB);

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

        cv::resize(image, image, cv::Size(), 0.5, 0.5);
        cv::imshow("out", image);
        char key = (char)cv::waitKey(20);
        if (key == 27)
            break;
    }

    return 0;
}
