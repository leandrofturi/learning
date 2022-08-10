#include <iostream>
#include <aruco/aruco.h>
#include <opencv2/highgui.hpp>

cv::RNG rng(12345);

int main(int argc, char **argv)
{
    try
    {
        int XSize = 1, YSize = 1;

        auto Dict = aruco::Dictionary::loadPredefined("ARUCO_MIP_36h12");
        int pixSize = 0.035 * 3779.5275590551; // meter to pixel
        float interMarkerDistance = 0.2;

        std::vector<int> ids = {1};

        aruco::MarkerMap BInfo =
            Dict.createMarkerMap(cv::Size(XSize, YSize), pixSize, static_cast<int>(float(pixSize) * interMarkerDistance), ids, false);
        // create image
        cv::Mat MarkerMapImage = BInfo.getImage();

        // border
        cv::copyMakeBorder(MarkerMapImage, MarkerMapImage, 100, 100, 100, 100, cv::BORDER_CONSTANT, 255);

        // save
        BInfo.saveToFile("board_configuration_0.035.yml");
        imwrite("board_0.035.png", MarkerMapImage);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}
