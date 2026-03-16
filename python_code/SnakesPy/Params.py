from enum import Enum
import json
import os

class SnakeSolver(Enum):
    GREEDY_SOLVER = 1
    EULER_LAGRANGE_SOLVER = 2

class Params:
    def __init__(self, path=None):
        # Always use params.json in the current working dir unless a path is given
        if path is None:
            path = os.path.join(os.getcwd(), 'params.json')
        self.path = path

        try:
            self.loadparams_from_json(self.path)
        except Exception:
            # If no params file exists, create one with default params
            self.alpha = 0.5
            self.beta = 0.5
            self.max_iterations = 500
            self.contour_points = 50
            self.solver = SnakeSolver.EULER_LAGRANGE_SOLVER
            self.saveParamsAsJson(self.alpha, self.beta, self.max_iterations, self.contour_points)

    def loadparams_from_json(self,path: str):
        with open(path, 'r') as json_file:
            data = json.load(json_file)
            self.alpha = data.get("alpha", 0.5)
            self.beta = data.get("beta", 0.5)
            self.max_iterations = data.get("max_iterations", 500)
            self.contour_points = data.get("Contourpoints", 50)
            self.solver = SnakeSolver[data.get("solver", "EULER_LAGRANGE_SOLVER")]

    def saveParamsAsJson(self, alpha : float, beta : float, max_iterations : int, Contourpoints: int) -> None:
        dict = {
            "alpha": alpha,
            "beta": beta,
            "Contourpoints": Contourpoints,
            "max_iterations": max_iterations,
            "solver": self.solver.name
        }
        with open(self.path, 'w') as json_file:
            json.dump(dict, json_file, indent=4)