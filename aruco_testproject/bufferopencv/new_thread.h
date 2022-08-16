#include <opencv2/opencv.hpp>

using namespace cv;

class new_thread
{
public:
    new_thread();         // constructor
    void get_frame(void); // grab frame
    Mat return_frame();   // return frame to main.cpp
    VideoCapture cap;
    Mat frame; // frmae i
};