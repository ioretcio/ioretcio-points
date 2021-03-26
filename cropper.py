import numpy as np
import cv2
import sys


img = cv2.imread(sys.argv[1])


height, width, channels = img.shape





multiplicatorDX = 1
multiplicatorDY = 1

PositionX = 0
PositionY = 0



while(1):
    crop_img = img[y:y+h, x:x+w]