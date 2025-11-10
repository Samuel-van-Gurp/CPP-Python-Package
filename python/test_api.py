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
    # ensure correct dtype & memory layout
    img = np.ascontiguousarray(img, dtype=np.uint8)

    height, width = img.shape
    stride_bytes = img.strides[0]               # bytes per row (NumPy)
    stride_elems = stride_bytes // img.itemsize # elements per row (what C++ wants)

    data_ptr = img.ctypes.data_as(ctypes.POINTER(ctypes.c_uint8))

    # set these once at module init ideally (safe to keep here for now)
    lib.transverImage.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.c_int, ctypes.c_int, ctypes.c_int]
    lib.transverImage.restype = None

    # pass width, height, and stride in elements (not bytes)
    # debug print: confirm what we're sending
    print("PY->C: dtype", img.dtype, "shape", img.shape, "strides(bytes)", img.strides, "stride_elems", stride_elems)
    # print first 10 bytes of the first row for quick verification
    first_row = img[0, :min(10, img.shape[1])].tolist()
    print("PY->C: first row bytes:", first_row)

    lib.transverImage(data_ptr, width, height, stride_elems)


lib.hello_get_message.restype = c_char_p
lib.get_info_from_cpp.restype = None

CoinImage = Image.Image("StarImage.png")
# CoinImage = Image.Image("coin.jpg")
# CoinImage.downsampleImage(6)
# CoinImage.show_image()
CoinArray = CoinImage.convert_to_numpy()
# print("Numpy array shape:", CoinArray.shape)
send_grayscale_image_to_cpp(CoinArray)
