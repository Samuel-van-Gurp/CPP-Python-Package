from ctypes import Structure, c_float, c_int, POINTER, c_uint8, c_int32


class SnakeParams(Structure):
    _fields_ = [
        ("alpha", c_float),
        ("beta", c_float),
        ("iterations", c_int),
        ("contour_center_x", c_float),
        ("contour_center_y", c_float),
        ("contour_radius_x", c_float),
        ("contour_radius_y", c_float),
        ("contour_points", c_int),
    ]

class ImageInfo(Structure):
    _fields_ = [
        ("data", POINTER(c_uint8)),
        ("width", c_int),
        ("height", c_int),
        ("stride", c_int),
    ]

class Point(Structure):
    _fields_ = [
        ("X", c_float),
        ("Y", c_float),
    ]