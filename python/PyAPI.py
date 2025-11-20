from ctypes import  POINTER, byref, cdll, c_uint8, Structure, c_float, c_int
import numpy as np  
import os
from DataClasses.DataClasses import SnakeParams, ImageInfo, Point



class PyAPI:
    def __init__(self, snakeParams : SnakeParams, imageInfo: ImageInfo):
        self.snakeParams = snakeParams
        self.imageInfo = imageInfo

        self.outSizeContours = c_int(0)
        
        dll_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../build/Debug/hello_c_api.dll"))

        self.lib = cdll.LoadLibrary(dll_path)

    def setImageInfo(self, img: np.ndarray) -> None:
        self.imageInfo.width = c_int(img.shape[1])
        self.imageInfo.height = c_int(img.shape[0])
        stride_bytes = img.strides[0]               # bytes per row (NumPy)
        self.imageInfo.stride = c_int(stride_bytes // img.itemsize) 
        self.imageInfo.data = img.ctypes.data_as(POINTER(c_uint8))


    def setSnakeParams(self, alpha : float, beta: float, iterations: int, center_x: float, center_y: float, radius: float, points: int) -> None:
        self.snakeParams.alpha = c_float(alpha)
        self.snakeParams.beta = c_float(beta)
        self.snakeParams.iterations = c_int(iterations)
        self.snakeParams.contour_center_x = c_float(center_x)
        self.snakeParams.contour_center_y = c_float(center_y)
        self.snakeParams.contour_radius = c_float(radius)
        self.snakeParams.contour_points = c_int(points)

    def reconstructCountours(self, contour_ptr, size_contours: c_int) -> np.ndarray:
        
        # construct an array of pairs (x, y)
        contours = np.zeros((size_contours, 2), dtype=np.int32)

        n = int(size_contours)                    # ensure plain int
        contours = np.zeros((n, 2), dtype=np.int32)
        for i in range(n):
            contours[i, 0] = int(contour_ptr[i].X)
            contours[i, 1] = int(contour_ptr[i].Y)
        return contours
    

    def callApi(self, img: np.ndarray) -> np.ndarray:

        img = np.ascontiguousarray(img, dtype=np.uint8)

        out_contour = (Point * self.snakeParams.contour_points)() # allocate array for output contour points

        self.lib.runSnake.argtypes = [POINTER(ImageInfo),    
                                      POINTER(SnakeParams),
                                      POINTER(Point),
                                      POINTER(c_int)]

        self.lib.runSnake.restype = None
        contour_ptr = self.lib.runSnake(byref(self.imageInfo), byref(self.snakeParams), 
                                                               out_contour,
                                                               byref(self.outSizeContours))

        return self.reconstructCountours(out_contour, self.outSizeContours.value) 
        


        
        



            