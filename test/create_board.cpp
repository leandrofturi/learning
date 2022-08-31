#include <iostream>
#include <aruco/aruco.h>


using namespace std;
using namespace cv;

// ./run board.png configuration.yml
int main(int argc, char **argv)
{
    try
    {
        int XSize = 5, YSize = 3;
        auto Dict = aruco::Dictionary::load("ARUCO_MIP_36h12");
        int pixSize = 500;
        int interMarkerDistance = 100;

        std::vector<int> ids = {1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0};
        // std::vector<int> ids = {9, 10, 11, 12, 13, 14, 15, 16, 0, 0, 0, 0, 0, 0, 0};
        ids.resize(XSize * YSize);

        aruco::MarkerMap BInfo =
            Dict.createMarkerMap(Size(XSize, YSize), pixSize, interMarkerDistance, ids, true);
        // create a printable image to save
        cv::Mat MarkerMapImage = BInfo.getImage();
        cv::copyMakeBorder(MarkerMapImage, MarkerMapImage, 100, 100, 100, 100, cv::BORDER_CONSTANT, 255);

        // save
        BInfo.saveToFile(argv[2]);
        imwrite(argv[1], MarkerMapImage);
    }
    catch (std::exception &ex)
    {
        cout << ex.what() << endl;
    }
}
