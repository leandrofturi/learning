#include <iostream>
#include <bitset>
#include <aruco/aruco.h>
#include <opencv2/highgui.hpp>


aruco::MarkerMap createMarkerMap(cv::Size gridSize, int MarkerSize, int MarkerDistance, const std::vector<int> &ids, bool chess_board)
{
    aruco::MarkerMap TInfo;

    TInfo.mInfoType = aruco::MarkerMap::PIX;
    TInfo.setDictionary("CUSTOM");
    const float markerSizeFloat = static_cast<float>(MarkerSize);

    if (!chess_board)
    {
        TInfo.resize(ids.size());
        for (size_t i = 0; i < ids.size(); i++)
            TInfo[i].id = ids[i];
        int idp = 0;
        float mtotal = static_cast<float>(MarkerDistance) + markerSizeFloat;
        for (int y = 0; y < gridSize.height; y++)
            for (int x = 0; x < gridSize.width; x++, idp++)
            {
                /// \todo use const auto &
                for (auto p : aruco::Marker::get3DPoints(markerSizeFloat))
                    TInfo[idp].points.push_back(p + cv::Point3f(x * mtotal, y * mtotal, 0));
            }
    }
    else
    {

        // find the center so that the ref systeem is in it
        int CurMarkerIdx = 0;
        for (int y = 0; y < gridSize.height; y++)
        {

            bool toWrite;
            if (y % 2 == 0)
                toWrite = false;
            else
                toWrite = true;
            for (int x = 0; x < gridSize.width; x++)
            {
                toWrite = !toWrite;
                if (toWrite)
                {
                    if (CurMarkerIdx >= int(ids.size()))
                        throw cv::Exception(999, " FiducidalMarkers::createMarkerMapImage_ChessMarkerMap", "INTERNAL ERROR. REWRITE THIS!!", __FILE__, __LINE__);
                    TInfo.push_back(aruco::Marker3DInfo(ids[CurMarkerIdx++]));
                    /// \todo use const auto &
                    for (auto p : aruco::Marker::get3DPoints(markerSizeFloat))
                        TInfo.back().points.push_back(p + cv::Point3f(x * markerSizeFloat, y * markerSizeFloat, 0));
                }
            }
        }
    }

    //  cv::Point3f center(0,0,0);
    //  double n=0;
    //  for(auto &ti:TInfo)for(auto p:ti) {center+=p;n++;}
    //  center*=1./n;
    cv::Point3f center(gridSize.width / 2.0f * markerSizeFloat - markerSizeFloat / 2.0f, gridSize.height / 2.0f * markerSizeFloat - markerSizeFloat / 2.0f, 0.f);
    for (auto &ti : TInfo)
        for (auto &p : ti.points)
            p -= center;

    return TInfo;
}

int main(int argc, char **argv)
{
    try
    {
        int XSize = 1, YSize = 1;

        // auto Dict = aruco::Dictionary::loadPredefined("ARUCO_MIP_36h12");
        auto Dict = aruco::Dictionary::loadFromFile("../dic_4x4-50.txt");
        int pixSize = 300;
        float interMarkerDistance = 0.2;

        std::vector<int> ids = {1};

        aruco::MarkerMap BInfo = createMarkerMap(cv::Size(XSize, YSize), pixSize, static_cast<int>(float(pixSize) * interMarkerDistance), ids, false);
        // create image
        cv::Mat MarkerMapImage = BInfo.getImage();

        // border
        // cv::copyMakeBorder(MarkerMapImage, MarkerMapImage, 100, 100, 100, 100, cv::BORDER_CONSTANT, 255);

        // save
        // BInfo.saveToFile("board_configuration_300.yml");
        // imwrite("board_300.png", MarkerMapImage);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}
