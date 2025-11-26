import json


class Params:
    def __init__(self, path = "Params/params.json"):

        self.path = path

        try:
            self.loadparams_from_json(path)
        except Exception as e:
            print(f"Failed to load params from {path}: {e}")
            self.alpha = 0.5
            self.beta = 0.5
            self.max_iterations = 500
            self.contour_points = 50

    def loadparams_from_json(self,path: str):
        with open(path, 'r') as json_file:
            data = json.load(json_file)
            self.alpha = data.get("alpha", 0.5)
            self.beta = data.get("beta", 0.5)
            self.max_iterations = data.get("max_iterations", 500)
            self.contour_points = data.get("Contourpoints", 50)

    def saveParamsAsJson(self, alpha : float, beta : float, max_iterations : int, Contourpoints: int) -> None:
        dict = {
            "alpha": alpha,
            "beta": beta,
            "Contourpoints": Contourpoints,
            "max_iterations": max_iterations
        }

        with open('Params/params.json', 'w') as json_file:
            json.dump(dict, json_file, indent=4)