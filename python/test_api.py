from ctypes import cdll, c_char_p, POINTER, c_uint8
import ctypes
import os
from matplotlib.widgets import EllipseSelector, PolygonSelector
import numpy as np
import Image
import PyAPI
import matplotlib.pyplot as plt
from DataClasses.DataClasses import SnakeParams, ImageInfo
from UI import UI

ui = UI()

CoinImage = Image.Image("StarImage.png")
# CoinImage = Image.Image("coin.jpg")
# CoinImage.downsampleImage(6)
# CoinImage.show_image()

CoinArray = CoinImage.convert_to_numpy()

ui.selectInitContour(CoinArray)

print("Size of CoinArray:", CoinArray.shape)
pyAPI = PyAPI.PyAPI(SnakeParams(), ImageInfo())

pyAPI.setImageInfo(CoinArray)

pyAPI.setSnakeParams(alpha= 0.5, 
                    beta=0.4, 
                    iterations=500, 
                    center_x=ui.xy[0], 
                    center_y=ui.xy[1], 
                    radius_x=ui.width / 2, 
                    radius_y=ui.height / 2, 
                    points=50)

contour = pyAPI.callApi(CoinArray)
ui.displayImageWithContour(CoinArray, contour)