#include <opencv2/opencv.hpp>
#include "new_thread.h" //this is a custom class that required for grab frames(we dedicate a new thread for this class)
#include <QtConcurrent/QtConcurrent>
#include <QThread> //need for generate artificial delay, which in real situation it will produce by weak processor or etc.
#include <iostream>

using namespace cv;

int main()
{
    Mat frame;
    new_thread th;                                  // create an object from new_thread class
    QtConcurrent::run(&th, &new_thread::get_frame); // start a thread with QtConcurrent
    QThread::msleep(2000);                          // need some time to start(open) camera
    while (true)
    {
        frame = th.return_frame();
        imshow("frame", frame);
        waitKey(20);
        // QThread::msleep(20); // artifical delay, or a big process
        // std::cout << th.cap.get(cv::CAP_PROP_BUFFERSIZE) << std::endl;
    }
}