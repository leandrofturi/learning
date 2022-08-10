#include <iostream>
//#include <eigen3/Eigen/Core>
#include <opencv2/highgui.hpp>
//#include <opencv2/core/eigen.hpp>
#include <aruco/aruco.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: inimage" << std::endl;
        return -1;
    }
    aruco::CameraParameters CamParam;
    CamParam.readFromXMLFile("../calibration.yml");

    cv::Mat image = cv::imread(argv[1]);
    aruco::MarkerDetector MDetector;

    MDetector.setDictionary("ARUCO_MIP_36h12");

    // detect
    std::vector<aruco::Marker> markers = MDetector.detect(image, CamParam, 0.035);
    // std::vector<aruco::Marker> markers = MDetector.detect(image);
    // print info to console

    float rx = 0.0, ry = 0.0, rz = 0.0;
    float N = (float)markers.size();
    for (size_t i = 0; i < markers.size(); i++)
    {
        // std::cout << markers[i] << std::endl;
        // draw in the image
        markers[i].draw(image);

        aruco::CvDrawingUtils::draw3dAxis(image, CamParam, markers[i].Rvec, markers[i].Tvec, 0.035);

        // CV_32F: float, CV_64F: double
        rx += markers[i].Rvec.at<float>(0) * 57.2958 / N;
        ry += markers[i].Rvec.at<float>(1) * 57.2958 / N;
        rz += markers[i].Rvec.at<float>(2) * 57.2958 / N;

        // Eigen::Matrix<float, 3, 1> rvec;
        // cv::cv2eigen(markers[i].Rvec, rvec);
        // std::cout << rvec * 57.2958 << std::endl;
    }

    std::cout << rx << "," << ry << "," << rz << std::endl;
    cv::imshow("image", image);
    cv::waitKey(0);
}
