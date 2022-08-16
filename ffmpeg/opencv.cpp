#include <iostream>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    Mat image;
    VideoCapture vcap("rtsp://admin:1q2w3e4r@192.168.1.114:554/cam/realmonitor?channel=1&subtype=0");

    while (1)
    {
        vcap.read(image);
        imshow("image", image);
        waitKey(1);
    }

    return(0);
}
