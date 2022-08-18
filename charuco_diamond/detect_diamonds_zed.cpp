#include <sl/Camera.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco/charuco.hpp>

#include <vector>
#include <iostream>

#include "utils_aruco.hpp"

int main(int argc, char *argv[])
{
    float squareLength = 0.48; // Square side length (in meters)
    float markerLength = 0.3;  // Marker side length (in meters)

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
    cv::Mat distCoeffs = (cv::Mat_<double>(8, 1) << k1, k2, p1, p2, k3, k4, k5, k6);

    // https://docs.opencv.org/4.x/d1/dcd/structcv_1_1aruco_1_1DetectorParameters.html
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();
    FileStorage fs("../detector_params.yml", FileStorage::READ);
    readDetectorParameters(fs.root(), detectorParams);

    std::cout << detectorParams->adaptiveThreshConstant << std::endl;

    // https://docs.opencv.org/3.4/d9/d6a/group__aruco.html
    cv::Ptr<cv::aruco::Dictionary> dictionary;
    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME(0));

    cv::Vec3d rvecMean = {.0, .0, .0},
              tvecMean = {.0, .0, .0},
              rvecStd = {.0, .0, .0},
              tvecStd = {.0, .0, .0};
    size_t i;

    while (zed.grab() == sl::ERROR_CODE::SUCCESS)
    {
        zed.retrieveImage(image_zed, sl::VIEW::LEFT, sl::MEM::CPU, image_size);
        cv::cvtColor(image_ocv, image, cv::COLOR_RGBA2RGB);

        vector<int> markerIds;
        vector<Vec4i> diamondIds;
        vector<vector<Point2f>> markerCorners, chessboardCorners, rejectedMarkers, diamondCorners;
        vector<Vec3d> rvecs, tvecs;

        // detect markers
        cv::aruco::detectMarkers(image, dictionary, markerCorners, markerIds, detectorParams,
                                 rejectedMarkers);

        if (markerIds.size() > 0)
        {
            aruco::detectCharucoDiamond(image, markerCorners, markerIds,
                                        squareLength / markerLength, diamondCorners, diamondIds,
                                        camMatrix, distCoeffs);
        }

        // estimate diamond pose
        int diamondIdSize = diamondIds.size();
        if (diamondIdSize > 0)
        {
            aruco::estimatePoseSingleMarkers(diamondCorners, squareLength, camMatrix,
                                             distCoeffs, rvecs, tvecs);
        }
        else
        {
            aruco::estimatePoseSingleMarkers(markerCorners, squareLength, camMatrix,
                                             distCoeffs, rvecs, tvecs);
        }

        if (markerIds.size() > 0)
        {
            aruco::drawDetectedMarkers(image, markerCorners);
        }

        if (rejectedMarkers.size() > 0)
        {
            aruco::drawDetectedMarkers(image, rejectedMarkers, noArray(), Scalar(255, 0, 255));
        }

        size_t rvecsize = rvecs.size();
        if (diamondIdSize > 0)
        {
            aruco::drawDetectedDiamonds(image, diamondCorners, diamondIds);
        }

        rvecMean = {.0, .0, .0};
        tvecMean = {.0, .0, .0};
        rvecStd = {.0, .0, .0};
        tvecStd = {.0, .0, .0};
        for (i = 0; i < rvecsize; i++)
        {
            aruco::drawAxis(image, camMatrix, distCoeffs, rvecs[i], tvecs[i],
                            squareLength * 0.5f);
            rvecMean += rvecs[i] / (int)rvecsize;
            tvecMean += tvecs[i] / (int)rvecsize;
        }

        for (i = 0; i < rvecsize; i++)
        {
            rvecStd += (rvecs[i] - rvecMean).mul(rvecs[i] - rvecMean);
            tvecStd += (tvecs[i] - tvecMean).mul(tvecs[i] - tvecMean);
        }
        sqrt((rvecStd / (int)rvecsize), rvecStd);
        sqrt((tvecStd / (int)rvecsize), tvecStd);

        rvecMean = rvecMean * 57.2958;
        std::cout << rvecMean << "std: " << rvecStd << std::endl;

        cv::imshow("out", image);
        char key = (char)cv::waitKey(1);
        if (key == 27)
            break;
    }

    return 0;
}
