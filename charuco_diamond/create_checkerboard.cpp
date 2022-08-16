#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    int blockSize = 100;
    int imageSize = blockSize * 8;
    Mat chessBoard(imageSize, imageSize, CV_8UC3, Scalar::all(0));
    unsigned char color = 0;

    for (int i = 0; i < imageSize; i = i + blockSize)
    {
        color = ~color;
        for (int j = 0; j < imageSize; j = j + blockSize)
        {
            Mat ROI = chessBoard(Rect(i, j, blockSize, blockSize));
            ROI.setTo(Scalar::all(color));
            color = ~color;
        }
    }

    imshow("chessboard", chessBoard);
    imwrite("../chessboard.jpg", chessBoard);
    waitKey(0);
    return(0);
}
