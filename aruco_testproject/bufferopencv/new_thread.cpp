#include "new_thread.h"
#include <QThread>
#include <opencv2/opencv.hpp>

new_thread::new_thread()
{
    cap.open("rtsp://admin:1q2w3e4r@192.168.1.114:554/cam/realmonitor?channel=1&subtype=0"); // open camera when class is constructing
    cap.set(cv::CAP_PROP_BUFFERSIZE, 3.0);
}

void new_thread::get_frame() // get frame method
{
    while (1) // while(1) because we want to grab frames continuously
    {
        cap.read(frame);
    }
}

Mat new_thread::return_frame()
{
    return frame; // whenever main.cpp need updated frame it can request last frame by usign this method
}