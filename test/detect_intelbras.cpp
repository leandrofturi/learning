#include <iostream>
#include <vector>
// FFmpeg
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}
// OpenCV
#include <opencv2/opencv.hpp>
#include <aruco/aruco.h>

typedef struct tAv_ffmpeg
{
    AVFormatContext *inctx;
    int vstrm_idx;
    AVStream *vstrm;
    AVCodecContext *avctx;
    SwsContext *swsctx;
    AVFrame *frame;
    std::vector<uint8_t> *framebuf;
    AVFrame *decframe;
    unsigned nb_frames;
    bool end_of_stream;
    AVPacket pkt;
} av_ffmpeg;

int init(av_ffmpeg *afp, const char *infile)
{
    int ret;

    // open input file context
    AVFormatContext *inctx = nullptr;
    ret = avformat_open_input(&inctx, infile, nullptr, nullptr);
    if (ret < 0)
    {
        std::cout << "fail to avforamt_open_input(\"" << infile << "\"): ret=" << ret;
        return 2;
    }
    // retrive input stream information
    ret = avformat_find_stream_info(inctx, nullptr);
    if (ret < 0)
    {
        std::cout << "fail to avformat_find_stream_info: ret=" << ret;
        return 2;
    }
    // find primary video stream
    AVCodec *vcodec = nullptr;
    ret = av_find_best_stream(inctx, AVMEDIA_TYPE_VIDEO, -1, -1, &vcodec, 0);
    if (ret < 0)
    {
        std::cout << "fail to av_find_best_stream: ret=" << ret;
        return 2;
    }
    const int vstrm_idx = ret;
    AVStream *vstrm = inctx->streams[vstrm_idx];
    // open video decoder context
    AVCodec *pCodec = avcodec_find_decoder(vstrm->codecpar->codec_id);
    AVCodecContext *avctx = avcodec_alloc_context3(pCodec);
    avcodec_parameters_to_context(avctx, vstrm->codecpar);

    vstrm->time_base = avctx->time_base;

    ret = avcodec_open2(avctx, vcodec, nullptr);
    if (ret < 0)
    {
        std::cout << "fail to avcodec_open2: ret=" << ret;
        return 2;
    }

    // print input video stream informataion
    std::cout
        << "format: " << inctx->iformat->name << "\n"
        << "vcodec: " << vcodec->name << "\n"
        << "size:   " << avctx->width << 'x' << avctx->height << "\n"
        << "fps:    " << av_q2d(avctx->framerate) << " [fps]\n"
        << "length: " << av_rescale_q(vstrm->duration, vstrm->time_base, {1, 1000}) / 1000. << " [sec]\n"
        << "pixfmt: " << av_get_pix_fmt_name(avctx->pix_fmt) << "\n"
        << "frame:  " << vstrm->nb_frames << "\n"
        << std::flush;

    // initialize sample scaler
    const int dst_width = avctx->width;
    const int dst_height = avctx->height;
    const AVPixelFormat dst_pix_fmt = AV_PIX_FMT_BGR24;

    SwsContext *swsctx = sws_getCachedContext(
        nullptr, avctx->width, avctx->height, avctx->pix_fmt,
        dst_width, dst_height, dst_pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
    if (!swsctx)
    {
        std::cout << "fail to sws_getCachedContext";
        return 2;
    }
    std::cout << "output: " << dst_width << 'x' << dst_height << ',' << av_get_pix_fmt_name(dst_pix_fmt) << std::endl;

    // allocate frame buffer for output
    AVFrame *frame = av_frame_alloc();
    std::vector<uint8_t> *framebuf = new std::vector<uint8_t>(av_image_get_buffer_size(dst_pix_fmt, dst_width, dst_height, 1));
    av_image_fill_arrays(frame->data, frame->linesize, framebuf->data(), dst_pix_fmt, dst_width, dst_height, 1);

    afp->inctx = inctx;
    afp->vstrm_idx = vstrm_idx;
    afp->vstrm = vstrm;
    afp->avctx = avctx;
    afp->swsctx = swsctx;
    afp->frame = frame;
    afp->framebuf = framebuf;

    // decoding loop
    afp->decframe = av_frame_alloc();
    afp->nb_frames = 0;
    afp->end_of_stream = false;

    return ret;
}

unsigned char *get_image(av_ffmpeg *afp)
{
    int ret;
    const int dst_width = afp->avctx->width;
    const int dst_height = afp->avctx->height;

    if (!afp->end_of_stream)
    {
        // read packet from input file
        ret = av_read_frame(afp->inctx, &afp->pkt);
        if (ret < 0 && ret != AVERROR_EOF)
        {
            std::cout << "fail to av_read_frame: ret=" << ret;
            return nullptr;
        }
        if (ret == 0 && afp->pkt.stream_index != afp->vstrm_idx)
        {
            av_packet_unref(&afp->pkt);
            return nullptr;
        }
        afp->end_of_stream = (ret == AVERROR_EOF);
    }
    if (afp->end_of_stream)
    {
        // null packet for bumping process
        av_init_packet(&afp->pkt);
        afp->pkt.data = nullptr;
        afp->pkt.size = 0;
    }
    // decode video frame
    ret = avcodec_receive_frame(afp->avctx, afp->decframe);
    ret = avcodec_send_packet(afp->avctx, &afp->pkt);
    if (ret < 0)
    {
        std::cout << "fail to avcodec_send_packet: ret=" << ret;
        av_packet_unref(&afp->pkt);
        return nullptr;
    }
    // convert frame to OpenCV matrix
    sws_scale(afp->swsctx, afp->decframe->data, afp->decframe->linesize, 0, afp->decframe->height, afp->frame->data, afp->frame->linesize);

    ++afp->nb_frames;

    cv::Mat image(dst_height, dst_width, CV_8UC3, afp->framebuf->data(), afp->frame->linesize[0]);

    av_packet_unref(&afp->pkt);

    // std::cout << afp->nb_frames << " | " << afp->frame->linesize[0] << '\r' << std::flush; // dump progress

    return image.data;
}

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
        mmap = mmap.convertToMeters(marker_size_in_meters);

        mmap.getIdList(ids);

        MMTracker.setParams(camera, mmap, marker_size_in_meters);
        Detector.setDictionary(mmap.getDictionary());
        name = _name;
    }

    void detect(cv::Mat &image)
    {
        std::vector<aruco::Marker> markers = Detector.detect(image);

        std::vector<int> markers_from_set = mmap.getIndices(markers);
        for (auto idx : markers_from_set)
            markers[idx].draw(image, cv::Scalar(0, 0, 255), 1.5);
        if (MMTracker.estimatePose(markers))
        {
            std::cout << "[" << name << "] R: " << MMTracker.getRvec() * 57.2958 << "\tT: " << MMTracker.getTvec() << std::endl;

            // +X is Right on the screen, +Y is Up, +Z is INTO the screen
            aruco::CvDrawingUtils::draw3dAxis(image, camera, MMTracker.getRvec(), MMTracker.getTvec(), mmap[0].getMarkerSize());
        }
    }
};

int main(int argc, char *argv[])
{
    const char *infile = "rtsp://admin:1q2w3e4r@192.168.1.114:554/cam/realmonitor?channel=1&subtype=0";
    float marker_size_in_meters = 0.052;
    av_ffmpeg *afp = (av_ffmpeg *)malloc(sizeof(av_ffmpeg));
    int ret;
    unsigned char *raw;

    init(afp, infile);

    const int dst_width = afp->avctx->width;
    const int dst_height = afp->avctx->height;

    float fx = 8.7744289698736577e+02,
          fy = 6.3745148125019182e+02,
          cx = 8.7666465139596107e+02,
          cy = 3.6802907731718176e+02;

    double k1 = -9.1606622110679548e-02,
           k2 = 8.8697307066021214e-01,
           p1 = -9.3680993299164288e-03,
           p2 = -1.9210448543725927e-02,
           k3 = -3.3517554468041704e+00;

    cv::Mat camMatrix = (cv::Mat_<double>(3, 3) << fx, 0., cx, 0., fy, cy, 0., 0., 1.);
    cv::Mat distCoeffs = (cv::Mat_<double>(5, 1) << k1, k2, p1, p2, k3);
    cv::Size size(image_size.height, image_size.width);

    PoseTracker pt1(camMatrix, distCoeffs, size, "../configuration1.yml", marker_size_in_meters, "1");
    PoseTracker pt2(camMatrix, distCoeffs, size, "../configuration2.yml", marker_size_in_meters, "2");
    PoseTracker pt3(camMatrix, distCoeffs, size, "../configuration3.yml", marker_size_in_meters, "3");
    PoseTracker pt4(camMatrix, distCoeffs, size, "../configuration4.yml", marker_size_in_meters, "4");

    do
    {
        raw = get_image(afp);
        cv::Mat image(cv::Size(dst_width, dst_height), CV_8UC3, raw);
        cv::imshow("intelbras", image);
        cv::waitKey(1)
    } while (!afp->end_of_stream);

    av_frame_free(&afp->decframe);
    av_frame_free(&afp->frame);
    avcodec_free_context(&afp->avctx);
    avformat_close_input(&afp->inctx);

    free(afp->framebuf);
    free(afp);

    return 0;
}
