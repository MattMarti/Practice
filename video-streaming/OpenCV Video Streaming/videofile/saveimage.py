import cv2

vidcap = cv2.VideoCapture('The-Bee-Movie-but-gets-faster.mp4')
success, image = vidcap.read()

count = 0

success = True

while success:
    success, image = vidcap.read()
    if not count % 1000:
        cv2.imwrite("frame%d.jpg" % count, image)
    count+=1