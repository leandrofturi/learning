#include <opencv2/highgui.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

/*
./create_diamond "../diamond.jpg"
*/
int main(int argc, char *argv[])
{
    int squareLength = 160; // square side length (in pixels)
    int markerLength = 100; // marker side length (in pixels)
    int dictionaryId = 0;
    /*
        DICT_4X4_50=0, DICT_4X4_100=1, DICT_4X4_250=2,
        DICT_4X4_1000=3, DICT_5X5_50=4, DICT_5X5_100=5, DICT_5X5_250=6, DICT_5X5_1000=7,
        DICT_6X6_50=8, DICT_6X6_100=9, DICT_6X6_250=10, DICT_6X6_1000=11, DICT_7X7_50=12,
        DICT_7X7_100=13, DICT_7X7_250=14, DICT_7X7_1000=15, DICT_ARUCO_ORIGINAL=16
    */
    Vec4i ids = {8, 9, 10, 11};
    int margins = 100;  // margins size (in pixels)
    int borderBits = 1; // number of bits in marker borders
    String out = argv[1];

    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(dictionaryId));

    Mat markerImg;
    aruco::drawCharucoDiamond(dictionary, ids, squareLength, markerLength, markerImg, margins,
                              borderBits);

    imshow("board", markerImg);
    waitKey(0);

    imwrite(out, markerImg);

    return 0;
}
