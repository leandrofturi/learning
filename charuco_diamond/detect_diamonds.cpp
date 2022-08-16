#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

// ./detect_diamonds <file> or ./detect_diamonds, opening the camera
int main(int argc, char *argv[])
{
    float squareLength = 0.48; // Square side length (in meters)
    float markerLength = 0.3;  // Marker side length (in meters)
    string camId = "rtsp://admin:1q2w3e4r@192.168.1.114:554/cam/realmonitor?channel=1&subtype=0";

    // https://docs.opencv.org/4.x/d1/dcd/structcv_1_1aruco_1_1DetectorParameters.html
    Ptr<aruco::DetectorParameters> detectorParams;
    FileStorage fs("../detector_params.yml", FileStorage::READ);
    cout << "readDetectorParameters: " << aruco::DetectorParameters::readDetectorParameters(fs.root(), detectorParams) << endl;

    // https://docs.opencv.org/3.4/d9/d6a/group__aruco.html
    Ptr<aruco::Dictionary> dictionary;
    dictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(0));

    Mat camMatrix = (cv::Mat_<double>(3, 3) << 903.40589687, 0., 630.54593345, 0., 900.68916931, 365.73991155, 0., 0., 1.);
    Mat distCoeffs = (cv::Mat_<double>(5, 1) << 1.58349076e-01, -3.66657961e+00, -6.04817151e-03, -1.92630777e-02, 4.11148177e+01);

    VideoCapture inputVideo;
    int waitTime;
    if (argc >= 2)
    {
        inputVideo.open(argv[1]);
        waitTime = 0;
    }
    else
    {
        inputVideo.open(camId);
        waitTime = 10;
    }

    // double tick = (double)getTickCount();

    Vec3d rvecMean = {.0, .0, .0},
          tvecMean = {.0, .0, .0},
          rvecStd = {.0, .0, .0},
          tvecStd = {.0, .0, .0};

    vector<int>::iterator first;
    vector<int>::iterator last;
    int min = 0;
    int max = 0;
    size_t i;

    // double totalTime = 0;
    // int totalIterations = 0;

    while (inputVideo.grab())
    {
        Mat image, imageCopy;
        inputVideo.retrieve(image);

        vector<int> markerIds;
        vector<Vec4i> diamondIds;
        vector<vector<Point2f>> markerCorners, chessboardCorners, rejectedMarkers, diamondCorners;
        vector<Vec3d> rvecs, tvecs;

        // detect markers
        aruco::detectMarkers(image, dictionary, markerCorners, markerIds, detectorParams,
                             rejectedMarkers);

        // detect diamonds
        if (markerIds.size() > 0)
        {
            if (rvecStd[2] < 1e-3)
            {
                first = markerIds.begin();
                last = markerIds.end();

                min = *min_element(first, last);
                max = *max_element(first, last);

                if (min < 4 && max > 4) // only if has two tables
                {
                    for (i = 0; i < markerIds.size(); i++)
                    {
                        if (rvecMean[2] < 15 && markerIds[i] < 4)
                        {
                            markerIds.erase(markerIds.begin() + i);
                            markerCorners.erase(markerCorners.begin() + i);
                        }
                        else if (rvecMean[2] > 15 && markerIds[i] >= 4)
                        {
                            markerIds.erase(markerIds.begin() + i);
                            markerCorners.erase(markerCorners.begin() + i);
                        }
                    }
                }
            }

            if (markerIds.size() > 0)
            {
                aruco::detectCharucoDiamond(image, markerCorners, markerIds,
                                            squareLength / markerLength, diamondCorners, diamondIds,
                                            camMatrix, distCoeffs);
            }
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

        /*
        double currentTime = ((double)getTickCount() - tick) / getTickFrequency();
        totalTime += currentTime;
        totalIterations++;
        if (totalIterations % 30 == 0)
        {
            cout << "Detection Time = " << currentTime * 1000 << " ms "
                 << "(Mean = " << 1000 * totalTime / double(totalIterations) << " ms)" << endl;
        }
        */

        // draw results
        image.copyTo(imageCopy);
        if (markerIds.size() > 0)
        {
            aruco::drawDetectedMarkers(imageCopy, markerCorners);
        }

        if (rejectedMarkers.size() > 0)
        {
            aruco::drawDetectedMarkers(imageCopy, rejectedMarkers, noArray(), Scalar(255, 0, 255));
        }

        int rvecsize = rvecs.size();
        if (diamondIdSize > 0)
        {
            aruco::drawDetectedDiamonds(imageCopy, diamondCorners, diamondIds);
        }

        rvecMean = {.0, .0, .0};
        tvecMean = {.0, .0, .0};
        rvecStd = {.0, .0, .0};
        tvecStd = {.0, .0, .0};
        for (i = 0; i < rvecsize; i++)
        {
            aruco::drawAxis(imageCopy, camMatrix, distCoeffs, rvecs[i], tvecs[i],
                            squareLength * 0.5f);
            rvecMean += rvecs[i] / rvecsize;
            tvecMean += tvecs[i] / rvecsize;
        }

        for (i = 0; i < rvecsize; i++)
        {
            rvecStd += (rvecs[i] - rvecMean).mul(rvecs[i] - rvecMean);
            tvecStd += (tvecs[i] - tvecMean).mul(tvecs[i] - tvecMean);
        }
        sqrt((rvecStd / rvecsize), rvecStd);
        sqrt((tvecStd / rvecsize), tvecStd);

        rvecMean = rvecMean * 57.2958;
        cout << rvecMean << "std: " << rvecStd << endl;

        imshow("out", imageCopy);
        char key = (char)waitKey(waitTime);
        if (key == 27)
            break;
    }

    return 0;
}
