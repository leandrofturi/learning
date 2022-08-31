#include <carmen/carmen.h>
#include <carmen/bumblebee_basic_interface.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "PoseTracker.hpp"


float scale = 0.5;
int buffer_size = 10;
PoseTracker pose_tracker;
cv::Mat Rvec_buf(3, 1, CV_32FC1, 0.);

float marker_size_in_meters = 0.118;
char board_config_files[1024];
int show_output = 0;


void
read_parameters(int argc, char **argv)
{
    carmen_param_allow_unfound_variables(1);
	carmen_param_t optional_commandline_param_list[] =
	{
		{(char *) "commandline", (char *) "m",  CARMEN_PARAM_INT,    &marker_size_in_meters, 0, NULL},
		{(char *) "commandline", (char *) "c", CARMEN_PARAM_DOUBLE, &board_config_files,       0, NULL},
        {(char *) "commandline", (char *) "show", CARMEN_PARAM_INT,    &show_output,              0, NULL}
	};
	carmen_param_install_params(argc, argv, optional_commandline_param_list, sizeof(optional_commandline_param_list) / sizeof(optional_commandline_param_list[0]));
}

static int show_left = 1;
static int show_right = 0;
static int received_image = 0;
static carmen_bumblebee_basic_stereoimage_message last_message;
static int msg_fps = 0, msg_last_fps = 0; //message fps
static int disp_fps = 0, disp_last_fps = 0; //display fps

void
update_fps(carmen_bumblebee_basic_stereoimage_message* image_msg)
{
    static double last_timestamp = 0.0;
    static double last_time = 0.0;
    double time_now = carmen_get_time();

    if (!received_image)
    {
        received_image = 1;
        last_timestamp = image_msg->timestamp;
        last_time = time_now;
    }

    if ((image_msg->timestamp - last_timestamp) > 1.0)
    {
        msg_last_fps = msg_fps;
        msg_fps = 0;
        last_timestamp = image_msg->timestamp;
    }

    msg_fps++;

    if ((time_now - last_time) > 1.0)
    {
        disp_last_fps = disp_fps;
        disp_fps = 0;
        last_time = time_now;
    }

    disp_fps++;

    last_message = *image_msg;
}


int count = 0;
void
detect(cv::Mat image)
{
	static int first = 1;
    cv::Mat bgr, cropped;
    static PoseTracker pose_tracker_translation;
    static cv::Mat Rvec_buf_inc(1, 3, CV_32FC1, 0.);

    int width = image.size().width, height = image.size().height;

    if (first)
    {
		cv::Mat camMatrix = (cv::Mat_<double>(3, 3) << 704.595458984375, .0, 662.9486083984375, .0, 704.595458984375, 372.1501770019531, .0, .0, 1.);
		cv::Mat distCoeffs = (cv::Mat_<double>(5, 1) << .0, .0, .0, .0, .0);
		pose_tracker.init(camMatrix, distCoeffs, image.size(), "/home/lume/carmen_lcad/src/aruco/board_configuration_1.yml", marker_size_in_meters);
        // different images
        pose_tracker_translation.init(camMatrix, distCoeffs, image.size(), "/home/lume/carmen_lcad/src/aruco/board_configuration_1.yml", marker_size_in_meters);

    	first = 0;
    }

    // maior detected
    int n_markers_detected = pose_tracker_translation.detect(image, false);
    if (n_markers_detected)
    {
        cv::Point2f center_aruco = pose_tracker_translation.getCenter();
        // digital zoom
        int center_X = center_aruco.x, 
            center_Y = center_aruco.y, 
            diam_X = scale * width, 
            diam_Y = scale * height, 
            crop_x = center_X - diam_X / 2, 
            crop_y = center_Y - diam_Y / 2;

        if (crop_x < 0) crop_x = 0;
        if (crop_y < 0) crop_y = 0;
        if ((crop_x + diam_X) > width) crop_x = width - diam_X;
        if ((crop_y + diam_Y) > height) crop_y = height - diam_Y;

        // cv::circle(image, center_aruco, 10, cv::Scalar(255, 255, 0));

        cv::Rect myROI(crop_x, crop_y, diam_X, diam_Y);
        cropped = image(myROI);
        // cropped = image;

        cv::resize(cropped, cropped, cv::Size(width, height));
        cv::cvtColor(cropped, bgr, cv::COLOR_BGR2RGB);
        // cv::rectangle(bgr, myROI, cv::Scalar(255, 255, 0));

        if (pose_tracker.detect(bgr))
        {
            if (count < buffer_size)
                Rvec_buf = pose_tracker.getRvec();
            Rvec_buf_inc += pose_tracker.getRvec() / buffer_size;
            count++;
        }
            

        if (count % buffer_size == 0)
        {
            Rvec_buf = Rvec_buf_inc.clone();
            Rvec_buf_inc = cv::Mat::zeros(1, 3, CV_32FC1);
        }
    }
    else
    {
        cv::cvtColor(image, bgr, cv::COLOR_BGR2RGB);
    }

    // last vecs
    std::cout << pose_tracker.getRvec() * 57.2958 << "\t" << Rvec_buf * 57.2958 << std::endl;

    cv::imshow("aruco", bgr);

    cv::waitKey(1);
}


void
image_handler(carmen_bumblebee_basic_stereoimage_message* msg)
{
    static char msg_fps_string[256];
    static char disp_fps_string[256];
    static char img_resol_string[256];
    cv::Mat src_image_left, src_image_right, text_image, concat;
    

    update_fps(msg);

    if (show_left)
    	src_image_left = cv::Mat(cv::Size(msg->width, msg->height), CV_8UC3, msg->raw_left);
    if (show_right)
    	src_image_right = cv::Mat(cv::Size(msg->width, msg->height), CV_8UC3, msg->raw_right);

    sprintf(msg_fps_string, "MSG_FPS: %d", msg_last_fps);
    sprintf(disp_fps_string, "DISP_FPS: %d", disp_last_fps);
    sprintf(img_resol_string, "%dx%d", msg->width, msg->height);

    text_image = show_left ? src_image_left : src_image_right;
    putText(text_image, msg_fps_string, cv::Point(7, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 0));
    putText(text_image, disp_fps_string, cv::Point(7, 42), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 0));
    putText(text_image, img_resol_string, cv::Point(7, 64), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 0));

    if (show_left && show_right)
    	cv::hconcat(src_image_left, src_image_right, concat);
    else
    	concat = show_left ? src_image_left : src_image_right;

    detect(concat);
}


void
shutdown_module(int signo)
{
    if (signo == SIGINT)
	{
        carmen_ipc_disconnect();
        cv::destroyAllWindows();

        printf("Signal %d received, exiting program ...\n", signo);
        exit(0);
    }
}


void
check_parameters(int argc, char **argv)
{
    /*
	if (argc < 2)
	{
		carmen_die("---------------------------------------------\n Wrong number of parameters! \n---------------------------------------------\n\n"
        "USAGE: %s -m <marker_size_in_meters> -c <board_config_file_1>,<board_config_file_2> [-show]\n\n", argv[0]);
		exit (0);
	}
    */
}


void
subscribe_messages()
{
    carmen_bumblebee_basic_subscribe_stereoimage(1, NULL, (carmen_handler_t) image_handler, CARMEN_SUBSCRIBE_LATEST);
}


int
main(int argc, char **argv)
{
	// check_parameters(argc, argv);

	carmen_ipc_initialize(argc, argv);
	carmen_param_check_version(argv[0]);

	signal(SIGINT, shutdown_module);

    // read_parameters(argc, argv);

	subscribe_messages();
	carmen_ipc_dispatch();

	return 0;
}
