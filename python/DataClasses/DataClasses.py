from dataclasses import dataclass
import numpy as np

@dataclass
class SnakeParams:
    alpha: float
    beta: float
    iterations: int
    contour_center_x: float
    contour_center_y: float
    contour_radius_x: float
    contour_radius_y: float
    contour_points: int

@dataclass
class ImageInfo:
    data: np.ndarray
    width: int
    height: int
    stride: int

@dataclass
class Point:
    X: float
    Y: float