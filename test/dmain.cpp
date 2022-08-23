#include <sl/Camera.hpp>
#include <opencv2/opencv.hpp>

//#include "markermap.h"
#include "diamond.h"

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
    cv::Mat distCoeffs = (cv::Mat_<double>(5, 1) << k1, k2, p1, p2, k3);//, k4, k5, k6);
    cv::Size size(1280, 720);

    //MarkerMap mmap(camMatrix, distCoeffs, size);
    Diamond diamond(camMatrix, distCoeffs, size);

    while (zed.grab() == sl::ERROR_CODE::SUCCESS)
    {
        zed.retrieveImage(image_zed, sl::VIEW::LEFT, sl::MEM::CPU, image_size);
        cv::cvtColor(image_ocv, image, cv::COLOR_RGBA2RGB);

        //mmap.detect(image);
        diamond.detect(image);
    }

    return 0;
}
