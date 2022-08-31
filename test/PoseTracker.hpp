#ifndef POSETRACKER_H
#define POSETRACKER_H

#include <aruco/aruco.h>
#include <opencv2/highgui.hpp>

class PoseTracker
{
private:
    cv::Mat _rvec;
    cv::Mat _tvec;

    std::vector<int> ids;
    aruco::MarkerMap mmap;
    aruco::CameraParameters camera;
    aruco::MarkerDetector Detector;
    aruco::MarkerMapPoseTracker MMTracker;

public:
    void init(cv::Mat camMatrix, cv::Mat distCoeffs, cv::Size size, const char *filename, float marker_size_in_meters)
    {
        camera.setParams(camMatrix, distCoeffs, size);
        mmap.readFromFile(filename);
        mmap = mmap.convertToMeters(marker_size_in_meters);

        mmap.getIdList(ids);

        MMTracker.setParams(camera, mmap, marker_size_in_meters);
        Detector.setDictionary(mmap.getDictionary());

        _rvec = cv::Mat::zeros(1, 3, CV_32FC1);
        _tvec = cv::Mat::zeros(1, 3, CV_32FC1);
    }

    int detect(cv::Mat &image, bool show = true)
    {
        std::vector<aruco::Marker> markers = Detector.detect(image);

        std::vector<int> markers_from_set = mmap.getIndices(markers);
        if (show)
            for (auto idx : markers_from_set)
                markers[idx].draw(image, cv::Scalar(0, 0, 255), 1.5);
        if (MMTracker.estimatePose(markers))
        {
            _rvec = MMTracker.getRvec();
            _tvec = MMTracker.getTvec();

            // +X is Right on the screen, +Y is Up, +Z is INTO the screen
            if (show)
                aruco::CvDrawingUtils::draw3dAxis(image, camera, _rvec, _tvec, mmap[0].getMarkerSize());

            return markers_from_set.size();
        }
        return 0;
    }

    cv::Point2f getCenter()
    {
        float axis_size = mmap[0].getMarkerSize();

        cv::Mat objectPoints(4, 3, CV_32FC1);
        objectPoints.at<float>(0, 0) = 0;
        objectPoints.at<float>(0, 1) = 0;
        objectPoints.at<float>(0, 2) = 0;
        objectPoints.at<float>(1, 0) = axis_size;
        objectPoints.at<float>(1, 1) = 0;
        objectPoints.at<float>(1, 2) = 0;
        objectPoints.at<float>(2, 0) = 0;
        objectPoints.at<float>(2, 1) = axis_size;
        objectPoints.at<float>(2, 2) = 0;
        objectPoints.at<float>(3, 0) = 0;
        objectPoints.at<float>(3, 1) = 0;
        objectPoints.at<float>(3, 2) = axis_size;

        std::vector<cv::Point2f> imagePoints;
        cv::projectPoints(objectPoints, _rvec, _tvec, camera.CameraMatrix, camera.Distorsion, imagePoints);
        
        return imagePoints[0];
    }

    cv::Mat getRvec()
    {
        return _rvec;
    }

    cv::Mat getTvec()
    {
        return _tvec;
    }

    void reset()
    {
        MMTracker.reset();
    }
};

#endif
