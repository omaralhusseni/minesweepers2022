from turtle import width
import cv2
import numpy as np
import time

from presp import change_presp
from visualize import CAMERA_IP

tracker = cv2.TrackerCSRT_create()
# video = cv2.VideoCapture(0)


# get video from live http feed
def set_ip(ip):
    global camera_ip, video
    camera_ip = ip
    video = cv2.VideoCapture(camera_ip)
    video.set(cv2.CAP_PROP_BUFFERSIZE, 3)
    time.sleep(2)
    return camera_ip

#malyana 5ara
def init_warped(points):
    ok, original_frame = video.read()
    original_frame = change_presp(original_frame, points)

    o_width, o_height, _ = original_frame.shape

    scale_percent = 50 # percent of original size
    width = int(original_frame.shape[1] * scale_percent / 100)
    height = int(original_frame.shape[0] * scale_percent / 100)
    dim = (height, width)
    resized = cv2.resize(original_frame, dim, interpolation = cv2.INTER_AREA)

    bbox = cv2.selectROI(resized)
    # nbbox = (int((bbox[0]/width)*o_width), int((bbox[1]/height)*o_height), int((bbox[2]/width)*o_width), int((bbox[3]/height)*o_height))
    nbbox = (int((bbox[0]/height)*o_height), int((bbox[1]/width)*o_width), int((bbox[2]/height)*o_height), int((bbox[3]/width)*o_width))

    ok = tracker.init(original_frame, nbbox)
    cv2.destroyAllWindows()
    return original_frame

def get_frame_warped(points):
    ok, frame = video.read()
    frame = change_presp(frame, points)
    ok, bbox = tracker.update(frame)

    if ok:
        (x, y, w, h) = [int(v) for v in bbox]
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2, 1)
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        return (frame, x, y, w, h)

    else:
        print("not founf")
        return (cv2.cvtColor(frame, cv2.COLOR_BGR2RGB), 0, 0, 0, 0)


