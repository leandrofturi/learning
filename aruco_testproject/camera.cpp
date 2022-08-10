#include <iostream>
#include <sstream>
#include <fstream>
#include <aruco/aruco.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define CV_FOURCC(a, b, c, d) cv::VideoWriter::fourcc(a, b, c, d)

using namespace std;

cv::Mat __resize(const cv::Mat &in, int width)
{
    if (in.size().width <= width)
        return in;
    float yf = float(width) / float(in.size().width);
    cv::Mat im2;
    cv::resize(in, im2, cv::Size(width, static_cast<int>(in.size().height * yf)));
    return im2;
}

int main(int argc, char **argv)
{
    try
    {
        char key = 0;
        float MarkerSize = 0.2;

        cv::Mat InImage;
        cv::VideoCapture VReader;
        aruco::MarkerDetector MDetector;
        aruco::CameraParameters CamParam;

        VReader.open(0);
        VReader.set(cv::CAP_PROP_AUTOFOCUS, 0);

        if (!VReader.isOpened())
        {
            cerr << "Could not open video" << endl;
            return -1;
        }
        // read first image to get the dimensions
        VReader >> InImage;

        VReader.set(cv::CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
        VReader.set(cv::CAP_PROP_AUTOFOCUS, 0);

        cout << "JJJJ" << InImage.size() << endl;

        // detector
        MDetector.setDictionary("ARUCO_MIP_36h12");
        MDetector.setDetectionMode(aruco::DM_NORMAL);

        cv::namedWindow("in", cv::WINDOW_NORMAL);
        cv::resizeWindow("in", 800, 600);

        // camera params
        CamParam.readFromXMLFile("../calibration.yml");

        do
        {
            // get image
            VReader.retrieve(InImage);

            //  detect
            vector<aruco::Marker> Markers = MDetector.detect(InImage, CamParam, MarkerSize);

            // for each marker, draw info and its boundaries in the image
            for (unsigned int i = 0; i < Markers.size(); i++)
            {
                // cout << Markers[i] << endl;
                Markers[i].draw(InImage, cv::Scalar(0, 0, 255), 2);
            }
            if (CamParam.isValid() && MarkerSize != -1)
            {
                for (unsigned int i = 0; i < Markers.size(); i++)
                {
                    if (Markers[i].id == 229 || Markers[i].id == 161)
                        cout << "Rvec " << Markers[i].id << " " << Markers[i].Rvec << endl;
                    aruco::CvDrawingUtils::draw3dAxis(InImage, Markers[i], CamParam);
                    // aruco::CvDrawingUtils::draw3dAxis(image, CamParam, markers[i].Rvec, markers[i].Tvec, 0.035);
                    aruco::CvDrawingUtils::draw3dCube(InImage, Markers[i], CamParam);
                }
            }

            // show input with augmented information
            cv::namedWindow("in", 1);
            cv::imshow("in", __resize(InImage, 1280));

            key = cv::waitKey(10);
        } while (key != 0 && VReader.grab());
    }
    catch (std::exception &ex)
    {
        std::cout << "Exception :" << ex.what() << std::endl;
    }
}