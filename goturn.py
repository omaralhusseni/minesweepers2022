import cv2
import numpy as np
import time
from imutils.video import videostream

tracker = cv2.TrackerGOTURN_create()
# video = cv2.VideoCapture('https://192.168.190.103:8080/video')
video = cv2.VideoCapture(0)

# video = videostream.VideoStream(src='http://192.168.190.103:8080', framerate=2).start()
time.sleep(1)

ok, frame = video.read()

bbox = cv2.selectROI(frame)

ok = tracker.init(frame, bbox)

while True:
    ok, frame = video.read()
    if not ok:
        break
    ok, bbox = tracker.update(frame)
    if ok:
        (x, y, w, h) = [int(v) for v in bbox]
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2, 1)
        # print("X",x,"Y",y)
    else:
        cv2.putText(frame, 'Error', (100, 0),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    cv2.imshow('Tracking', frame)
    if cv2.waitKey(1) & 0XFF == 27:
        break
cv2.destroyAllWindows()
