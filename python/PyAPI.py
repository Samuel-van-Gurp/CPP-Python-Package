from ctypes import  POINTER, byref, cdll, c_uint8, c_float, c_int
import numpy as np  
import os

from shapely import points
from DataClasses.DataClasses import SnakeParams, ImageInfo, Point
import sys
sys.path.insert(0, r"C:\Users\svangurp\Desktop\CPP_PY_package\build\Debug")  # adjust path to the .pyd location
import pybindings
from pybindings import SnakeParams, Point


class PyAPI_Pybind11:

    def __init__(self, snakeParams : pybindings.SnakeParams):
        self.snakeParams = snakeParams

    def setSnakeParams(self, alpha : float, beta: float, iterations: int, center_x: float, center_y: float, radius_x: float, radius_y: float, points: int) -> None:
        self.snakeParams.alpha = alpha
        self.snakeParams.beta = beta
        self.snakeParams.iterations = iterations
        self.snakeParams.contour_center_x = center_x
        self.snakeParams.contour_center_y = center_y
        self.snakeParams.contour_radius_x = radius_x
        self.snakeParams.contour_radius_y = radius_y
        self.snakeParams.contour_points = points
    
    def callApi(self, img: np.ndarray) -> np.ndarray:
        contour = pybindings.RunSnake(img, self.snakeParams)

        contour_array = np.array([[p.x, p.y] for p in contour], dtype=np.float32)
        return contour_array

# class PyAPIBase:
#     def setImageInfo(self, img: np.ndarray) -> None:
#         self.imageInfo.width = img.shape[1]
#         self.imageInfo.height = img.shape[0]
#         stride_bytes = img.strides[0]               # bytes per row (NumPy)
#         self.imageInfo.stride = stride_bytes // img.itemsize
#         self.imageInfo.data = img

#     def setSnakeParams(self, alpha : float, beta: float, iterations: int, center_x: float, center_y: float, radius_x: float, radius_y: float, points: int) -> None:
#         self.snakeParams.alpha = alpha
#         self.snakeParams.beta = beta
#         self.snakeParams.iterations = iterations
#         self.snakeParams.contour_center_x = center_x
#         self.snakeParams.contour_center_y = center_y
#         self.snakeParams.contour_radius_x = radius_x
#         self.snakeParams.contour_radius_y = radius_y
#         self.snakeParams.contour_points = points

#     def callApi(self, img: np.ndarray) -> np.ndarray:
#         pass



# class PyAPI_Ctypes(PyAPIBase):
#     def __init__(self, snakeParams : SnakeParams, imageInfo: ImageInfo):
#         self.snakeParams = snakeParams
#         self.imageInfo = imageInfo

#         self.outSizeContours = c_int(0)
        
#         dll_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../build/Debug/c_api.dll"))

#         self.lib = cdll.LoadLibrary(dll_path)

#     def setImageInfo(self, img: np.ndarray) -> None:
#         self.imageInfo.width = c_int(img.shape[1])
#         self.imageInfo.height = c_int(img.shape[0])
#         stride_bytes = img.strides[0]               # bytes per row (NumPy)
#         self.imageInfo.stride = c_int(stride_bytes // img.itemsize) 
#         self.imageInfo.data = img.ctypes.data_as(POINTER(c_uint8))


#     def setSnakeParams(self, alpha : float, beta: float, iterations: int, center_x: float, center_y: float, radius_x: float, radius_y: float, points: int) -> None:
#         self.snakeParams.alpha = c_float(alpha)
#         self.snakeParams.beta = c_float(beta)
#         self.snakeParams.iterations = c_int(iterations)
#         self.snakeParams.contour_center_x = c_float(center_x)
#         self.snakeParams.contour_center_y = c_float(center_y)
#         self.snakeParams.contour_radius_x = c_float(radius_x)
#         self.snakeParams.contour_radius_y = c_float(radius_y)
#         self.snakeParams.contour_points = c_int(points)

#     def reconstructCountours(self, contour_ptr, size_contours: c_int) -> np.ndarray:
        
#         # construct an array of pairs (x, y)
#         contours = np.zeros((size_contours, 2), dtype=np.int32)

#         n = int(size_contours)                    # ensure plain int
#         contours = np.zeros((n, 2), dtype=np.float32)
#         for i in range(n):
#             contours[i, 0] = contour_ptr[i].X
#             contours[i, 1] = contour_ptr[i].Y
#         return contours
    

#     def callApi(self, img: np.ndarray) -> np.ndarray:

#         img = np.ascontiguousarray(img, dtype=np.uint8)
		
#         # init output parameter
#         out_contour = (Point * self.snakeParams.contour_points)() 

#         # Define function signature 
#         self.lib.runSnake.argtypes = [POINTER(ImageInfo),    
#                                       POINTER(SnakeParams),
#                                       POINTER(Point),
#                                       POINTER(c_int)]
#         self.lib.runSnake.restype = None
        
#         # Call 
#         self.lib.runSnake(byref(self.imageInfo), byref(self.snakeParams), 
#                           out_contour,
#                           byref(self.outSizeContours))
        
#         # convert to python container and return
#         return self.reconstructCountours(out_contour, self.outSizeContours.value)








