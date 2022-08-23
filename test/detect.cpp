#include <sl/Camera.hpp>
#include <opencv2/opencv.hpp>
#include <aruco/aruco.h>


/*
#include <bitset>
std::vector<uint64_t> ARUCO_MIP_36h12 = {0xd2b63a09dUL, 0x6001134e5UL, 0x1206fbe72UL, 0xff8ad6cb4UL, 0x85da9bc49UL, 0xb461afe9cUL, 0x6db51fe13UL, 0x5248c541fUL, 0x8f34503UL, 0x8ea462eceUL, 0xeac2be76dUL, 0x1af615c44UL, 0xb48a49f27UL, 0x2e4e1283bUL, 0x78b1f2fa8UL, 0x27d34f57eUL, 0x89222fff1UL, 0x4c1669406UL, 0xbf49b3511UL, 0xdc191cd5dUL, 0x11d7c3f85UL, 0x16a130e35UL, 0xe29f27effUL, 0x428d8ae0cUL, 0x90d548477UL, 0x2319cbc93UL, 0xc3b0c3dfcUL, 0x424bccc9UL, 0x2a081d630UL, 0x762743d96UL, 0xd0645bf19UL, 0xf38d7fd60UL, 0xc6cbf9a10UL, 0x3c1be7c65UL, 0x276f75e63UL, 0x4490a3f63UL, 0xda60acd52UL, 0x3cc68df59UL, 0xab46f9daeUL, 0x88d533d78UL, 0xb6d62ec21UL, 0xb3c02b646UL, 0x22e56d408UL, 0xac5f5770aUL, 0xaaa993f66UL, 0x4caa07c8dUL, 0x5c9b4f7b0UL, 0xaa9ef0e05UL, 0x705c5750UL, 0xac81f545eUL, 0x735b91e74UL, 0x8cc35cee4UL, 0xe44694d04UL, 0xb5e121de0UL, 0x261017d0fUL, 0xf1d439eb5UL, 0xa1a33ac96UL, 0x174c62c02UL, 0x1ee27f716UL, 0x8b1c5ece9UL, 0x6a05b0c6aUL, 0xd0568dfcUL, 0x192d25e5fUL, 0x1adbeccc8UL, 0xcfec87f00UL, 0xd0b9dde7aUL, 0x88dcef81eUL, 0x445681cb9UL, 0xdbb2ffc83UL, 0xa48d96df1UL, 0xb72cc2e7dUL, 0xc295b53fUL, 0xf49832704UL, 0x9968edc29UL, 0x9e4e1af85UL, 0x8683e2d1bUL, 0x810b45c04UL, 0x6ac44bfe2UL, 0x645346615UL, 0x3990bd598UL, 0x1c9ed0f6aUL, 0xc26729d65UL, 0x83993f795UL, 0x3ac05ac5dUL, 0x357adff3bUL, 0xd5c05565UL, 0x2f547ef44UL, 0x86c115041UL, 0x640fd9e5fUL, 0xce08bbcf7UL, 0x109bb343eUL, 0xc21435c92UL, 0x35b4dfce4UL, 0x459752cf2UL, 0xec915b82cUL, 0x51881eed0UL, 0x2dda7dc97UL, 0x2e0142144UL, 0x42e890f99UL, 0x9a8856527UL, 0x8e80d9d80UL, 0x891cbcf34UL, 0x25dd82410UL, 0x239551d34UL, 0x8fe8f0c70UL, 0x94106a970UL, 0x82609b40cUL, 0xfc9caf36UL, 0x688181d11UL, 0x718613c08UL, 0xf1ab7629UL, 0xa357bfc18UL, 0x4c03b7a46UL, 0x204dedce6UL, 0xad6300d37UL, 0x84cc4cd09UL, 0x42160e5c4UL, 0x87d2adfa8UL, 0x7850e7749UL, 0x4e750fc7cUL, 0xbf2e5dfdaUL, 0xd88324da5UL, 0x234b52f80UL, 0x378204514UL, 0xabdf2ad53UL, 0x365e78ef9UL, 0x49caa6ca2UL, 0x3c39ddf3UL, 0xc68c5385dUL, 0x5bfcbbf67UL, 0x623241e21UL, 0xabc90d5ccUL, 0x388c6fe85UL, 0xda0e2d62dUL, 0x10855dfe9UL, 0x4d46efd6bUL, 0x76ea12d61UL, 0x9db377d3dUL, 0xeed0efa71UL, 0xe6ec3ae2fUL, 0x441faee83UL, 0xba19c8ff5UL, 0x313035eabUL, 0x6ce8f7625UL, 0x880dab58dUL, 0x8d3409e0dUL, 0x2be92ee21UL, 0xd60302c6cUL, 0x469ffc724UL, 0x87eebeed3UL, 0x42587ef7aUL, 0x7a8cc4e52UL, 0x76a437650UL, 0x999e41ef4UL, 0x7d0969e42UL, 0xc02baf46bUL, 0x9259f3e47UL, 0x2116a1dc0UL, 0x9f2de4d84UL, 0xeffac29UL, 0x7b371ff8cUL, 0x668339da9UL, 0xd010aee3fUL, 0x1cd00b4c0UL, 0x95070fc3bUL, 0xf84c9a770UL, 0x38f863d76UL, 0x3646ff045UL, 0xce1b96412UL, 0x7a5d45da8UL, 0x14e00ef6cUL, 0x5e95abfd8UL, 0xb2e9cb729UL, 0x36c47dd7UL, 0xb8ee97c6bUL, 0xe9e8f657UL, 0xd4ad2ef1aUL, 0x8811c7f32UL, 0x47bde7c31UL, 0x3adadfb64UL, 0x6e5b28574UL, 0x33e67cd91UL, 0x2ab9fdd2dUL, 0x8afa67f2bUL, 0xe6a28fc5eUL, 0x72049cdbdUL, 0xae65dac12UL, 0x1251a4526UL, 0x1089ab841UL, 0xe2f096ee0UL, 0xb0caee573UL, 0xfd6677e86UL, 0x444b3f518UL, 0xbe8b3a56aUL, 0x680a75cfcUL, 0xac02baea8UL, 0x97d815e1cUL, 0x1d4386e08UL, 0x1a14f5b0eUL, 0xe658a8d81UL, 0xa3868efa7UL, 0x3668a9673UL, 0xe8fc53d85UL, 0x2e2b7edd5UL, 0x8b2470f13UL, 0xf69795f32UL, 0x4589ffc8eUL, 0x2e2080c9cUL, 0x64265f7dUL, 0x3d714dd10UL, 0x1692c6ef1UL, 0x3e67f2f49UL, 0x5041dad63UL, 0x1a1503415UL, 0x64c18c742UL, 0xa72eec35UL, 0x1f0f9dc60UL, 0xa9559bc67UL, 0xf32911d0dUL, 0x21c0d4ffcUL, 0xe01cef5b0UL, 0x4e23a3520UL, 0xaa4f04e49UL, 0xe1c4fcc43UL, 0x208e8f6e8UL, 0x8486774a5UL, 0x9e98c7558UL, 0x2c59fb7dcUL, 0x9446a4613UL, 0x8292dcc2eUL, 0x4d61631UL, 0xd05527809UL, 0xa0163852dUL, 0x8f657f639UL, 0xcca6c3e37UL, 0xcb136bc7aUL, 0xfc5a83e53UL, 0x9aa44fc30UL, 0xbdec1bd3cUL, 0xe020b9f7cUL, 0x4b8f35fb0UL, 0xb8165f637UL, 0x33dc88d69UL, 0x10a2f7e4dUL, 0xc8cb5ff53UL, 0xde259ff6bUL, 0x46d070dd4UL, 0x32d3b9741UL, 0x7075f1c04UL, 0x4d58dbea0UL};
for (size_t i = 0; i < 30; i++)
        {
            std::string binary = std::bitset<36>(ARUCO_MIP_36h12[i]).to_string(); // to binary
            std::cout << binary << "\n";
        }
fromVector(codes,d._code_id);
        d._nbits=36;
        d._tau=12;
        d._type=ARUCO_MIP_36h12;
        d._name="ARUCO_MIP_36h12";
*/

class PoseTracker
{
public:
    const char *name;
    aruco::CameraParameters camera;
    aruco::MarkerMap mmap;
    std::vector<int> ids;
    aruco::MarkerMapPoseTracker MMTracker;
    aruco::MarkerDetector Detector;

    std::map<uint64_t, uint16_t> code_id;

    PoseTracker(cv::Mat camMatrix, cv::Mat distCoeffs, cv::Size size, const char *filename, float marker_size_in_meters, const char *_name)
    {
        camera.setParams(camMatrix, distCoeffs, size);
        mmap.readFromFile(filename);

        mmap.getIdList(ids);

        MMTracker.setParams(camera, mmap, marker_size_in_meters);
        Detector.setDictionary(mmap.getDictionary());
        name = _name;
    }

    void detect(cv::Mat& image)
    {
        std::vector<aruco::Marker> markers = Detector.detect(image);

        std::vector<int> markers_from_set = mmap.getIndices(markers);
        for (auto idx : markers_from_set)
            markers[idx].draw(image, cv::Scalar(0, 0, 255), 2);
        if (MMTracker.estimatePose(markers))
        {
            std::cout << "[" << name << "] R: " << MMTracker.getRvec() * 57.2958 << "\tT: " << MMTracker.getTvec() << std::endl;

            // +X is Right on the screen, +Y is Up, +Z is INTO the screen
            aruco::CvDrawingUtils::draw3dAxis(image, camera, MMTracker.getRvec(), MMTracker.getTvec(), mmap[0].getMarkerSize() * 2);
        }
    }
};

int main(int argc, char *argv[])
{
    float marker_size_in_meters = 0.035;
    // sl::Camera zed;
    cv::VideoCapture VReader;

    sl::InitParameters init_params;
    init_params.sdk_verbose = false;
    init_params.camera_resolution = sl::RESOLUTION::HD720;
    init_params.coordinate_units = sl::UNIT::METER;
    init_params.sensors_required = false;

    // sl::ERROR_CODE err = zed.open(init_params);
    // if (err != sl::ERROR_CODE::SUCCESS)
    //     exit(-1);

    VReader.open(0);
    VReader.set(cv::CAP_PROP_AUTOFOCUS, 0);

    /*
        // in pixels
        float fx = zed.getCameraInformation().calibration_parameters.left_cam.fx,
              fy = zed.getCameraInformation().calibration_parameters.left_cam.fy,
              cx = zed.getCameraInformation().calibration_parameters.left_cam.cx,
              cy = zed.getCameraInformation().calibration_parameters.left_cam.cy;

        double k1 = zed.getCameraInformation().calibration_parameters.left_cam.disto[0],
               k2 = zed.getCameraInformation().calibration_parameters.left_cam.disto[1],
               p1 = zed.getCameraInformation().calibration_parameters.left_cam.disto[2],
               p2 = zed.getCameraInformation().calibration_parameters.left_cam.disto[3],
               k3 = zed.getCameraInformation().calibration_parameters.left_cam.disto[4],
               k4 = zed.getCameraInformation().calibration_parameters.left_cam.disto[5],
               k5 = zed.getCameraInformation().calibration_parameters.left_cam.disto[6],
               k6 = zed.getCameraInformation().calibration_parameters.left_cam.disto[7];
        sl::Resolution image_size = zed.getCameraInformation().camera_resolution;
        image_zed = sl::Mat(image_size, sl::MAT_TYPE::U8_C4);
        image_ocv = cv::Mat(image_zed.getHeight(), image_zed.getWidth(), CV_8UC4, image_zed.getPtr<sl::uchar1>(sl::MEM::CPU));
        cv::Size size(image_size.width, image_size.height);

        cv::Mat camMatrix = (cv::Mat_<double>(3, 3) << fx, 0., cx, 0., fy, cy, 0., 0., 1.);
        cv::Mat distCoeffs = (cv::Mat_<double>(5, 1) << k1, k2, p1, p2, k3); //, k4, k5, k6);
    */

    cv::Size size(1280, 720);

    cv::Mat camMatrix = (cv::Mat_<double>(3, 3) << 500, 0., 500, 0., 500, 500, 0., 0., 1.);
    cv::Mat distCoeffs = (cv::Mat_<double>(5, 1) << 0, 0, 0, 0, 0); //, k4, k5, k6);

    PoseTracker pt1(camMatrix, distCoeffs, size, "../configuration1.yml", marker_size_in_meters, "1");
    PoseTracker pt2(camMatrix, distCoeffs, size, "../configuration2.yml", marker_size_in_meters, "2");
    PoseTracker pt3(camMatrix, distCoeffs, size, "../configuration3.yml", marker_size_in_meters, "3");
    PoseTracker pt4(camMatrix, distCoeffs, size, "../configuration4.yml", marker_size_in_meters, "4");

    cv::Mat image;
    while (VReader.grab()) // : (zed.grab() == sl::ERROR_CODE::SUCCESS))
    {
        VReader.retrieve(image);

        // zed.retrieveImage(image_zed, sl::VIEW::LEFT, sl::MEM::CPU, image_size);
        // cv::cvtColor(image_ocv, image, cv::COLOR_RGBA2RGB);

        pt1.detect(image);
        pt2.detect(image);
        pt3.detect(image);
        pt4.detect(image);

        cv::imshow("in", image);
        cv::waitKey(1);
    }

    return 0;
}
