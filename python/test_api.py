from ctypes import cdll, c_char_p, POINTER, c_uint8
import ctypes
import os
import numpy as np
import Image

dll_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../build/Debug/hello_c_api.dll"))

lib = ctypes.cdll.LoadLibrary(dll_path)

class SnakeParams(ctypes.Structure):
    _fields_ = [
        ("alpha", ctypes.c_float),
        ("beta", ctypes.c_float),
        ("iterations", ctypes.c_int),
        ("contour_center_x", ctypes.c_float),
        ("contour_center_y", ctypes.c_float),
        ("contour_radius", ctypes.c_float),
        ("contour_points", ctypes.c_int),
    ]

class ImageInfo(ctypes.Structure):
    _fields_ = [
        ("data", ctypes.POINTER(ctypes.c_uint8)),
        ("width", ctypes.c_int),
        ("height", ctypes.c_int),
        ("stride", ctypes.c_int),
    ]

def setImageInfo(img: np.ndarray) -> ImageInfo:
    info = ImageInfo()
    info.width = ctypes.c_int(img.shape[1])
    info.height = ctypes.c_int(img.shape[0])
    stride_bytes = img.strides[0]               # bytes per row (NumPy)
    info.stride = ctypes.c_int(stride_bytes // img.itemsize) 
    info.data = img.ctypes.data_as(ctypes.POINTER(ctypes.c_uint8))
    return info

def setSnakeParams(alpha : float, beta: float, iterations: int, center_x: float, center_y: float, radius: float, points: int) -> SnakeParams:
    params = SnakeParams()
    params.alpha = ctypes.c_float(alpha)
    params.beta = ctypes.c_float(beta)
    params.iterations = ctypes.c_int(iterations)
    params.contour_center_x = ctypes.c_float(center_x)
    params.contour_center_y = ctypes.c_float(center_y)
    params.contour_radius = ctypes.c_float(radius)
    params.contour_points = ctypes.c_int(points)
    return params

def send_grayscale_image_to_cpp(img: np.ndarray):

    img = np.ascontiguousarray(img, dtype=np.uint8)

    image_info = setImageInfo(img)
    
    params = setSnakeParams(alpha= 0.5, 
                            beta=0.4, 
                            iterations=100, 
                            center_x=140.0, 
                            center_y=100.0, 
                            radius=50.0, 
                            points=50)

    lib.PrepImageAndSnake.argtypes = [ctypes.POINTER(ImageInfo),    
                                      ctypes.POINTER(SnakeParams)]

    lib.PrepImageAndSnake.restype = None
    lib.PrepImageAndSnake(ctypes.byref(image_info), ctypes.byref(params))

CoinImage = Image.Image("StarImage.png")
# CoinImage = Image.Image("coin.jpg")
# CoinImage.downsampleImage(6)
# CoinImage.show_image()
CoinArray = CoinImage.convert_to_numpy()
send_grayscale_image_to_cpp(CoinArray)
