import os
import cv2

RTSP_URL = "rtsp://admin:1q2w3e4r@192.168.1.114:554/cam/realmonitor?channel=1\&subtype=0"

os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = "-flags low_delay;-vf setpts=0"
cap = cv2.VideoCapture(RTSP_URL, cv2.CAP_FFMPEG)

while True:
    _, frame = cap.read()
    cv2.imshow('RTSP stream', frame)

    if cv2.waitKey(1) == 27:
        break

cap.release()
cv2.destroyAllWindows()

"ffplay -flags low_delay -vf setpts=0 rtsp://admin:1q2w3e4r@192.168.1.114:554/cam/realmonitor?channel=1\&subtype=0"