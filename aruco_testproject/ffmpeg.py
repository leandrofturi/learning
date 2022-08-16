import ffmpeg
import cv2
import subprocess

RTSP_URL = "rtsp://admin:1q2w3e4r@192.168.1.114:554/cam/realmonitor?channel=1\&subtype=0"
server_url = "http://localhost:8081"

ffmpeg -thread_queue_size 1024 -input_format mjpeg -i rtsp://admin:1q2w3e4r@192.168.1.114:554/cam/realmonitor?channel=1&subtype=0 -r 30 -f alsa -ac 1 -thread_queue_size 1024 -i hw:1,0 -acodec aac -vcodec libx264 -preset ultrafast -crf 18 -s hd720 -vf format=yuv420p -profile:v main -threads 0 -f mpegts -|vlc -I dummy - --sout='#std{access=http,mux=ts,dst=:8554}'


def start_streaming():
    process = (
        ffmpeg
        .input(RTSP_URL)
        .output(
            server_url + '/stream',
            #codec = "copy", # use same codecs of the original video
            listen=1, # enables HTTP server
            pix_fmt="yuv420p",
            preset="ultrafast",
            f="flv"
        )
        .overwrite_output()
        .run_async(pipe_stdin=True)
    )
    return process

def init_cap():
    cap = cv2.VideoCapture(1)
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    return cap, width, height


cap, width, height = init_cap()
fps = cap.get(cv2.CAP_PROP_FPS)
streaming_process = start_streaming()
while True:

        streaming_process.stdin.write(frame.tobytes())
streaming_process.stdin.close()
streaming_process.wait()
cap.release()
