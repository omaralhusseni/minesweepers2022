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

    
    while len(points) != 4:
        ok, frame = cap.read()
        o_width, o_height, _ = frame.shape

        scale_percent = 50 # percent of original size
        width = int(frame.shape[0] * scale_percent / 100)
        height = int(frame.shape[1] * scale_percent / 100)
        dim = (height, width)
        resized = cv2.resize(frame, dim, interpolation = cv2.INTER_AREA)
        cv2.imshow('Calibrate, 1-bottom left 2-up left 3-bottom right 4-up right', resized)

        if ok:
            cv2.setMouseCallback("Calibrate, 1-bottom left 2-up left 3-bottom right 4-up right", handle_click)
            for p in points:
                cv2.circle(resized,(p[0],p[1]),10,(255,0,0),-1)

        else:
            cv2.putText(y, 'Error', (100, 0),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)


        cv2.imshow('Calibrate, 1-bottom left 2-up left 3-bottom right 4-up right', resized)
        if cv2.waitKey(1) & 0XFF == 27:
            break
    cv2.destroyAllWindows()

    for p in points:
        y, x = int((p[1]/height)*o_height), int((p[0]/width)*o_width)
        points[points.index(p)] = (x, y)

    return points
