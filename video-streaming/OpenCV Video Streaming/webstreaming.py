from pyimagesearch.motion_detection.singlemotiondetector import SingleMotionDetector
from imutils.video import VideoStream
from flask import Response
from flask import Flask
from flask import render_template
import threading
import argparse
import datetime
import imutils
import time
import cv2

# Initialize the output frame and a lock used to 
# ensure thread-safe exchanges
outputFrame = None
lock = threading.Lock() # Basically a mutex

# Initialize flask object
app = Flask(__name__)

# Initialize the video stream and allow camera to warm up
vs = VideoStream(src = 0).start() # USB webcam
#vs = VideoStream(usePiCamera = 1).start() # Raspberry Pi cam
time.sleep(2.0)

# Render index.html to serve output video stream
@app.route("/")
def index():
    # return rendered template
    return render_template("index.html")
#

# Loop over frames, apply motion detection, draw results
def detect_motion(frameCount):
    # Grab global references to video stream, output frame, and lock variables
    global vs, outputFrame, lock
    
    # Initialize motion detector and total number of frames read
    md = SingleMotionDetector(accumWeight = 0.1)
    total = 0
    
    # Start looping over frames
    while True:
        # Read next frame from video stream, resize, convert to grayscale, blur
        frame = vs.read()
        frame = imutils.resize(frame, width = 400)
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        gray = cv2.GaussianBlur(gray, (7, 7), 0)
        
        # Grab the current timestamp and draw it on the frame
        timestamp = datetime.datetime.now()
        cv2.putText(frame, timestamp.strftime(
            "%A %d %B %Y %I:%M:%S%p"), (10, frame.shape[0] - 10),
            cv2.FONT_HERSHEY_SIMPLEX, 0.35, (0, 0, 255), 1)
        #
        
        # If total number of frames is sufficient to construct the 
        # background model, then process the frame
        if total > frameCount:
            motion = md.detect(gray)
            
            # Check if motion was found
            if motion is not None:
                # Unpack tuple and draw the box surrounding the
                # motion area on output frame
                (thresh, (minX, minY, maxX, maxY)) = motion
                cv2.rectangle(frame, (minX, minY), (maxX, maxY), (0, 0, 255), 2)
            #
        #
        
        # Udpate background model and increment total number of frames
        md.update(gray)
        total += 1
        
        # Acquire lock (mutex), set the output frame, and release lock
        with lock:
            outputFrame = frame.copy()
        #
    #
#

# Encodes outputFrame as JPEG data
def generate():
    global outputFrame, lock
    
    # Loop over frames from output stream
    while True:
        with lock:
            if outputFrame is None: # If frame is not available, skip
                continue
            #
            
            # Encode the frame in JPEG
            (flag, encodedImage) = cv2.imencode(".jpg", outputFrame)
            
            # Ensure frame was successfully encoded
            if not flag:
                continue
            #
        #
        
        # Yield output frame in byte format
        yield(b'--frame\r\n' b'Content-Type: image/jpeg\r\n\r\n' + bytearray(encodedImage) + b'\r\n')
    #
#

@app.route("/video_feed") # This tells it that the function is a URL endpoint and data is being served
def video_feed():
    # Return response generated along with specific media type
    return Response(generate(), mimetype = "multipart/x-mixed-replace; boundary=frame")
#

# Parse command line arguments and launch Flask app
if __name__ == '__main__':
    # Argument parser
    ap = argparse.ArgumentParser()
    ap.add_argument("-i", "--ip", type=str, required=True, help="ip address of device")
    ap.add_argument("-o", "--port", type=int, required=True, help="Ephemeral port number of server (1024 to 65535)")
    ap.add_argument("-f", "--frame-count", type=int, default=32, help="# of frames used to construct the background model")
    args = vars(ap.parse_args())
    
    # Start thread that will perform motion detection
    t = threading.Thread(target = detect_motion, args = (args["frame_count"], )) # Run in background
    t.daemon = True
    t.start()
    
    # Start flask app
    app.run(host = args["ip"], port = args["port"], debug = True, threaded = True, use_reloader = False)
#

# Release video stream pointer
vs.stop()