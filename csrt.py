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


def init_normal():
    ok, frame = video.read()
    bbox = cv2.selectROI(frame)
    ok = tracker.init(frame, bbox)
    return frame

#malyana 5ara
def init_warped(points):
    ok, original_frame = video.read()
    frame = change_presp(original_frame, points)

    frame = cv2.rotate(original_frame, cv2.ROTATE_90_CLOCKWISE)

    bbox = cv2.selectROI("Select object", original_frame)

    frame = cv2.rotate(original_frame, cv2.ROTATE_90_COUNTERCLOCKWISE)

    bbox = [bbox[1], bbox[0], bbox[3], bbox[2]]

    ok = tracker.init(original_frame, bbox)
    cv2.destroyAllWindows()
    return original_frame


def get_frame():
    ok, frame = video.read()
    ok, bbox = tracker.update(frame)
    if ok:
        (x, y, w, h) = [int(v) for v in bbox]
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2, 1)
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        return frame

    else:
        print("not found")
        return frame


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



