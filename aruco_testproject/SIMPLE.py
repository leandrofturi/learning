import os
import cv2

RTSP_URL = "rtsp://admin:1q2w3e4r@192.168.1.114:554/cam/realmonitor?channel=1\&subtype=0"

os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = "-flags low_delay;-vf setpts=0"
cap = cv2.VideoCapture(RTSP_URL, cv2.CAP_FFMPEG)
cap

"ffplay -flags low_delay -vf setpts=0 rtsp://admin:1q2w3e4r@192.168.1.114:554/cam/realmonitor?channel=1\&subtype=0"

while True:
    _, frame = cap.read()
    cv2.imshow('RTSP stream', frame)

    if cv2.waitKey(1) == 27:
        break

cap.release()
cv2.destroyAllWindows()

"""


I had the same question bro)

Actually I've done some research, and there is my result

In openCV 4.5.1 property CAP_PROP_BUFFERSIZE is supported for some backends, but not for all. 
You can easily check if your backend supports this property. 
Just look at the result of the cap.get(CAP_PROP_BUFFERSIZE). 
If it returns 0, then your backend does not support this property. 
As good as I understand OpenCV's source code, only aravis, DC1394, V4L backends support this property

"""
