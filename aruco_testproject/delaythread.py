RTSP_URL = "rtsp://admin:1q2w3e4r@192.168.1.114:554/cam/realmonitor?channel=1\&subtype=0"

import cv2
print(cv2.__version__)
from threading import Thread

class ThreadedCamera(object):
    def __init__(self, source = 0):

        self.capture = cv2.VideoCapture(source)
        self.capture.set(cv2.CAP_PROP_BUFFERSIZE, 1) # n ta funcionando
        print(self.capture.get(cv2.CAP_PROP_BUFFERSIZE))

        self.thread = Thread(target = self.update, args = ())
        self.thread.daemon = True
        self.thread.start()

        self.status = False
        self.frame  = None

    def update(self):
        while True:
            if self.capture.isOpened():
                (self.status, self.frame) = self.capture.read()

    def grab_frame(self):
        if self.status:
            return self.frame
        return None

if __name__ == '__main__':
    streamer = ThreadedCamera(RTSP_URL)

    while True:
        frame = streamer.grab_frame()
        if frame is not None:
            cv2.imshow("Context", frame)
        cv2.waitKey(1)

"""


I had the same question bro)

Actually I've done some research, and there is my result

In openCV 4.5.1 property CAP_PROP_BUFFERSIZE is supported for some backends, but not for all. 
You can easily check if your backend supports this property. 
Just look at the result of the cap.get(CAP_PROP_BUFFERSIZE). 
If it returns 0, then your backend does not support this property. 
As good as I understand OpenCV's source code, only aravis, DC1394, V4L backends support this property

"""