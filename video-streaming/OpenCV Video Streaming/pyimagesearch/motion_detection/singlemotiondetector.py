import numpy as np
import imutils
import cv2

class SingleMotionDetector:
    def __init__(self, accumWeight = 0.5):
        self.accumWeight = accumWeight # larger means less background is factor in with the average
        
        # Initialize background model
        self.bg = None
    #
    
    # Accepts input frame and computes weighted average (filters it)
    def update(self, image):
        # If background model is None, then initialize it
        if self.bg is None:
            self.bg = image.copy().astype("float")
            return
        #
        
        # Update background model by accumulating weighted average
        cv2.accumulateWeighted(image, self.bg, self.accumWeight)
    #
    
    # Apply motion detection
    def detect(self, image, tVal = 25):
        # Compute difference between background model and image, and threshold the difference
        delta = cv2.absdiff(self.bg.astype("uint8"), image)
        thresh = cv2.threshold(delta, tVal, 255, cv2.THRESH_BINARY)[1]
        
        # Perform erosions and dilations to remove small blobs
        thresh = cv2.erode(thresh, None, iterations = 2)
        thresh = cv2.dilate(thresh, None, iterations = 2)
        
        # Apply contour detection to extract motion regions
        cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        cnts = imutils.grab_contours(cnts)
        (minX, minY) = (np.inf, np.inf)
        (maxX, maxY) = (-np.inf, -np.inf)
        
        # Populate variables
        if len(cnts) == 0: # No contours found
            return None
        #
        
        # Loop over contours
        for c in cnts:
            # Compute bounding box of contour and use it to update
            # minimum and maximum bounding box regions
            (x, y, w, h) = cv2.boundingRect(c)
            (minX, minY) = (min(minX, x), min(minY, y))
            (maxX, maxY) = (max(maxX, x + w), max(maxY, y + h))
        #
        
        # Return a tuple of thresholded image with bounding box
        return (thresh, (minX, minY, maxX, maxY))
    #
#