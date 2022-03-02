import cv2
import numpy as np

  
def change_presp(frame, points): 
    width, height, _ = frame.shape
    # Locate points of the documents
    # or object which you want to transform
    pts1 = np.float32(points)
                       
    pts2 = np.float32([[0, 0], [height, 0],
                       [0, width], [height, width]])
     
    # Apply Perspective Transform Algorithm
    matrix = cv2.getPerspectiveTransform(pts1, pts2)
    result = cv2.warpPerspective(frame, matrix, (width, height))
     

    result = cv2.rotate(result, cv2.ROTATE_90_COUNTERCLOCKWISE)
    result = cv2.rotate(result, cv2.ROTATE_90_COUNTERCLOCKWISE)
    
    return result