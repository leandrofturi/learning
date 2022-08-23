#include <vector>
#include <iostream>

#include "diamond.h"

template <typename T>
static inline bool readParameter(const cv::FileNode &node, T &parameter)
{
    if (!node.empty())
    {
        node >> parameter;
        return true;
    }
    return false;
}

bool readDetectorParameters(const cv::FileNode &fn, cv::Ptr<cv::aruco::DetectorParameters> &params)
{
    if (fn.empty())
        return true;

    bool checkRead = false;
    checkRead |= readParameter(fn["adaptiveThreshWinSizeMin"], params->adaptiveThreshWinSizeMin);
    checkRead |= readParameter(fn["adaptiveThreshWinSizeMax"], params->adaptiveThreshWinSizeMax);
    checkRead |= readParameter(fn["adaptiveThreshWinSizeStep"], params->adaptiveThreshWinSizeStep);
    checkRead |= readParameter(fn["adaptiveThreshConstant"], params->adaptiveThreshConstant);
    checkRead |= readParameter(fn["minMarkerPerimeterRate"], params->minMarkerPerimeterRate);
    checkRead |= readParameter(fn["maxMarkerPerimeterRate"], params->maxMarkerPerimeterRate);
    checkRead |= readParameter(fn["polygonalApproxAccuracyRate"], params->polygonalApproxAccuracyRate);
    checkRead |= readParameter(fn["minCornerDistanceRate"], params->minCornerDistanceRate);
    checkRead |= readParameter(fn["minDistanceToBorder"], params->minDistanceToBorder);
    checkRead |= readParameter(fn["minMarkerDistanceRate"], params->minMarkerDistanceRate);
    // checkRead |= readParameter(fn["doCornerRefinement"], params->doCornerRefinement);
    checkRead |= readParameter(fn["cornerRefinementWinSize"], params->cornerRefinementWinSize);
    checkRead |= readParameter(fn["cornerRefinementMaxIterations"], params->cornerRefinementMaxIterations);
    checkRead |= readParameter(fn["cornerRefinementMinAccuracy"], params->cornerRefinementMinAccuracy);
    checkRead |= readParameter(fn["markerBorderBits"], params->markerBorderBits);
    checkRead |= readParameter(fn["perspectiveRemovePixelPerCell"], params->perspectiveRemovePixelPerCell);
    checkRead |= readParameter(fn["perspectiveRemoveIgnoredMarginPerCell"], params->perspectiveRemoveIgnoredMarginPerCell);
    checkRead |= readParameter(fn["maxErroneousBitsInBorderRate"], params->maxErroneousBitsInBorderRate);
    checkRead |= readParameter(fn["minOtsuStdDev"], params->minOtsuStdDev);
    checkRead |= readParameter(fn["errorCorrectionRate"], params->errorCorrectionRate);
    return checkRead;
}

Diamond::Diamond(cv::Mat _camMatrix, cv::Mat _distCoeffs, cv::Size _size)
{
    camMatrix = _camMatrix;
    distCoeffs = _distCoeffs;
    size = _size;

    detectorParams = cv::aruco::DetectorParameters::create();
    cv::FileStorage fs("../detector_params.yml", cv::FileStorage::READ);
    readDetectorParameters(fs.root(), detectorParams);

    dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME(0));
}

void Diamond::detect(cv::Mat image)
{
    float squareLength = 0.048; // Square side length (in meters)
    float markerLength = 0.03;  // Marker side length (in meters)

    cv::Vec3d rvecMean = {.0, .0, .0},
              tvecMean = {.0, .0, .0};

    std::vector<cv::Vec3d> rvecs, tvecs;

    std::vector<int> markerIds;
    std::vector<cv::Vec4i> diamondIds;
    std::vector<std::vector<cv::Point2f>> markerCorners, chessboardCorners, rejectedMarkers, diamondCorners;

    // detect markers
    cv::aruco::detectMarkers(image, dictionary, markerCorners, markerIds, detectorParams,
                             rejectedMarkers);

    if (markerIds.size() > 0)
    {
        cv::aruco::detectCharucoDiamond(image, markerCorners, markerIds,
                                        squareLength / markerLength, diamondCorners, diamondIds,
                                        camMatrix, distCoeffs);
    }

    // estimate diamond pose
    int diamondIdSize = diamondIds.size();
    if (diamondIdSize > 0)
    {
        cv::aruco::estimatePoseSingleMarkers(diamondCorners, squareLength, camMatrix,
                                             distCoeffs, rvecs, tvecs);
    }
    else
    {
        cv::aruco::estimatePoseSingleMarkers(markerCorners, squareLength, camMatrix,
                                             distCoeffs, rvecs, tvecs);
    }

    if (markerIds.size() > 0)
    {
        cv::aruco::drawDetectedMarkers(image, markerCorners);
    }

    if (rejectedMarkers.size() > 0)
    {
        cv::aruco::drawDetectedMarkers(image, rejectedMarkers, cv::noArray(), cv::Scalar(255, 0, 255));
    }

    size_t rvecsize = rvecs.size();
    if (diamondIdSize > 0)
    {
        cv::aruco::drawDetectedDiamonds(image, diamondCorners, diamondIds);
    }

    for (size_t i = 0; i < rvecsize; i++)
    {
        cv::aruco::drawAxis(image, camMatrix, distCoeffs, rvecs[i], tvecs[i],
                            squareLength * 0.5f);
        rvecMean += rvecs[i] / (int)rvecsize;
        tvecMean += tvecs[i] / (int)rvecsize;
    }

    if (rvecsize > 0)
    {
        rvecMean = rvecMean * 57.2958;
        std::cout << "RVec: " << rvecMean << std::endl;
    }

    cv::imshow("diamond", image);
    char key = (char)cv::waitKey(1);
}
