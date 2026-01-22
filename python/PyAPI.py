import sys
import numpy as np  
import Config
sys.path.insert(0, str(Config.PYBINDINGS_DIR))
import pybindings
from pybindings import SnakeParams, Point

class Pybind11_API:

    def __init__(self, snakeParams : pybindings.SnakeParams):
        self.snakeParams = snakeParams

    @staticmethod
    def Pybind11_API_Factory(alpha : float, beta: float, iterations: int, 
                                                      center_x: float, 
                                                      center_y: float, 
                                                      radius_x: float, 
                                                      radius_y: float, 
                                                      points: int) -> Pybind11_API:
        snakeParams = SnakeParams()
        snakeParams.alpha = alpha
        snakeParams.beta = beta
        snakeParams.iterations = iterations
        snakeParams.contour_center_x = center_x
        snakeParams.contour_center_y = center_y
        snakeParams.contour_radius_x = radius_x
        snakeParams.contour_radius_y = radius_y
        snakeParams.contour_points = points

        return Pybind11_API(snakeParams)
    
    
    
    def callApi(self, img: np.ndarray) -> np.ndarray:
        contour = pybindings.RunSnake(img, self.snakeParams)

        contour_array = np.array([[p.x, p.y] for p in contour], dtype=np.float32)
        return contour_array
