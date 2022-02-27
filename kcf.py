from turtle import width
import cv2
import numpy as np
import time
from imutils.video import videostream

tracker = cv2.TrackerCSRT_create()
cap = cv2.VideoCapture(0)

# video = videostream.VideoStream(src='http://192.168.190.103:8080', framerate=2).start()
time.sleep(1)


tracker = cv2.TrackerKCF_create()
ok, frame = cap.read()

o_width, o_height, _ = frame.shape

scale_percent = 50 # percent of original size
width = int(frame.shape[1] * scale_percent / 100)
height = int(frame.shape[0] * scale_percent / 100)
dim = (width, height)
resized = cv2.resize(frame, dim, interpolation = cv2.INTER_AREA)

bbox = cv2.selectROI(resized)
nbbox = (int((bbox[0]/width)*o_width), int((bbox[1]/height)*o_height), int((bbox[2]/width)*o_width), int((bbox[3]/height)*o_height))

print(frame.shape)
print(resized.shape)
print(bbox)
print(nbbox)

ok = tracker.init(frame, nbbox)

while True:
    ok, frame = cap.read()
    if not ok:
        break
    ok, bbox = tracker.update(frame)
    if ok:
        (x, y, w, h) = [int(v) for v in bbox]
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2, 1)
    else:
        cv2.putText(frame, 'Error', (100, 0),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    cv2.imshow('Tracking', frame)
    if cv2.waitKey(1) & 0XFF == 27:
        break
cv2.destroyAllWindows()
