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


# CoinImage = Image.Image("StarImage.png")



CoinImage = Image.Image("coin.jpg")
CoinImage.downsampleImage(6)
# CoinImage.show_image()

CoinArray = CoinImage.convert_to_numpy()

ui.selectInitContour(CoinArray)

print("Size of CoinArray:", CoinArray.shape)
pyAPI = PyAPI.PyAPI(SnakeParams(), ImageInfo())

# pyAPI.setImageInfo(CoinArray)

# pyAPI.setSnakeParams(alpha= 0.5, 
#                     beta=0.4, 
#                     iterations=100, 
#                     center_x=140.0, 
#                     center_y=100.0, 
#                     radius=50.0, 
#                     points=50)

# contour = pyAPI.callApi(CoinArray)

# # print all points in the contour

# for point in contour:
#     print(f"Point: x={point[0]}, y={point[1]}")

# ui.displayImageWithContour(CoinArray, contour)




