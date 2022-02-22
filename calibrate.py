import cv2
import numpy as np
import time

# cap = cv2.VideoCapture(0)

global points
points = []

def handle_click(event, x, y, flags, params):
    if event == cv2.EVENT_LBUTTONDBLCLK:
        points.append((x, y))

def calibrate(HOST):
    cap = cv2.VideoCapture(HOST)
    time.sleep(3)
    ok, frame = cap.read()

    while len(points) != 4:
        cv2.imshow('Calibrate', frame)

        ok, frame = cap.read()
        if ok:
            cv2.setMouseCallback('Calibrate', handle_click)
            for p in points:
                cv2.circle(frame,(p[0],p[1]),10,(255,0,0),-1)

        else:
            cv2.putText(frame, 'Error', (100, 0),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)


        cv2.imshow('Calibrate', frame)
        if cv2.waitKey(1) & 0XFF == 27:
            break
    cv2.destroyAllWindows()

    return points
