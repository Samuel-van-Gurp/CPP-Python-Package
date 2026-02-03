import Image
import Params
import Config
import UI
import PyAPI

class ActiveContour:
    
    def __init__(self, path :str):  
        # TODO: this is a bit convoluted
        # TODO: check the path is valid
        self.ImageArray = Image.Image(path).convert_to_numpy()
        self.params = Params.Params()
        self.params.loadparams_from_json(Config.PARAMETER_JSON_PATH)
        self.ui = UI.UI()

    def run(self):
        self.ui.selectInitContour(self.ImageArray, alpha_init=self.params.alpha, beta_init=self.params.beta)
        api = PyAPI.Pybind11_API.Pybind11_API_Factory(
            alpha=self.ui.alphaUserInput.val,
            beta=self.ui.betaUserInput.val,
            iterations=self.ui.getItrerations(),
            center_x=self.ui.xy[0],
            center_y=self.ui.xy[1],
            radius_x=self.ui.width / 2,
            radius_y=self.ui.height / 2,
            points=self.ui.getPoints()
        )
        contour = api.callApi(self.ImageArray)
        self.ui.displayImageWithContour(self.ImageArray, contour)
        self.params.saveParamsAsJson(self.ui.alphaUserInput.val, self.ui.betaUserInput.val, self.ui.getItrerations(), self.ui.getPoints())