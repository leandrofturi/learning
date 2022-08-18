#include <opencv2/highgui.hpp>
#include <sl/Camera.hpp>
#include "utils_zed.hpp"

int main(int argc, char **argv)
{
    sl::Camera zed;

    sl::InitParameters init_params;
    init_params.sdk_verbose = true;

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
           k6 = zed.getCameraInformation().calibration_parameters.left_cam.disto[7], 
           s1 = zed.getCameraInformation().calibration_parameters.left_cam.disto[8], 
           s2 = zed.getCameraInformation().calibration_parameters.left_cam.disto[9], 
           s3 = zed.getCameraInformation().calibration_parameters.left_cam.disto[10], 
           s4 = zed.getCameraInformation().calibration_parameters.left_cam.disto[11];

    sl::Mat image_zed = sl::Mat(zed.getCameraInformation().camera_resolution, sl::MAT_TYPE::U8_C4);
    cv::Mat image_ocv = slMat2cvMat(image_zed);

    while (zed.grab() == sl::ERROR_CODE::SUCCESS)
    {
        zed.retrieveImage(image_zed, sl::VIEW::LEFT);
        cv::imshow("Image", image_ocv);
        cv::waitKey(1);
    }

    zed.close();
    return 0;
}
