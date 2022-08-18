#include <opencv2/aruco.hpp>

using namespace std;
using namespace cv;

template <typename T>
static inline bool readParameter(const FileNode &node, T &parameter)
{
    if (!node.empty())
    {
        node >> parameter;
        return true;
    }
    return false;
}

bool readDetectorParameters(const FileNode &fn, Ptr<aruco::DetectorParameters> &params)
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
    checkRead |= readParameter(fn["doCornerRefinement"], params->doCornerRefinement);
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