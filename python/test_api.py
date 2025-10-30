from ctypes import cdll, c_char_p, POINTER, c_uint8
import ctypes
import os
import numpy as np
import Image

dll_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../build/Debug/hello_c_api.dll"))
lib = cdll.LoadLibrary(dll_path)                                                

lib = ctypes.cdll.LoadLibrary(dll_path)

lib.hello_say_hello.argtypes = [ctypes.c_char_p]
lib.hello_say_hello.restype = None
lib.hello_say_hello(b"World") 

def send_grayscale_image_to_cpp(img: np.ndarray):

    img = np.ascontiguousarray(img)

    height, width = img.shape
    stride = img.strides[0]

    # Get a pointer to the image data
    data_ptr = img.ctypes.data_as(ctypes.POINTER(ctypes.c_uint8))

    lib.transverImage.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_int, ctypes.c_int, ctypes.c_int]
    lib.transverImage.restype = None

    lib.transverImage(data_ptr, width, height, stride)
 
lib.hello_get_message.restype = c_char_p
lib.get_info_from_cpp.restype = None

CoinImage = Image.Image("TestImage.png")
# CoinImage = Image.Image("coin.jpg")
# CoinImage.downsampleImage(4)
# CoinImage.show_image()
CoinArray = CoinImage.convert_to_numpy()
# print("Numpy array shape:", CoinArray.shape)
send_grayscale_image_to_cpp(CoinArray)
