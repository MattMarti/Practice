import time
import numpy as np
import cv2

cap = cv2.VideoCapture(0)

while True:
    # Frame by frame capture
    ret, frame = cap.read()
    
    # Operations on the frame come here
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Display
    cv2.imshow('frame', gray)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
        
    time.sleep(.1)
        
cap.release()
cv2.destroyAllWindows()