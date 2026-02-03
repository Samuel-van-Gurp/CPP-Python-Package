import Params
import PyAPI
import Config
from UI import UI

import Image
def main():
    # Create UI instance
    ui = UI()

    # Load image
    CoinImage = Image.Image("Images/astronaut.png")
    ImageArray = CoinImage.convert_to_numpy()

    # Load params from json
    params = Params.Params()
    params.loadparams_from_json(Config.PARAMETER_JSON_PATH)

    ui.selectInitContour(ImageArray, alpha_init=params.alpha, beta_init=params.beta)

    API = PyAPI.Pybind11_API.Pybind11_API_Factory(
        alpha=ui.alphaUserInput.val,
        beta=ui.betaUserInput.val,
        iterations=ui.getItrerations(),
        center_x=ui.xy[0],
        center_y=ui.xy[1],
        radius_x=ui.width / 2,
        radius_y=ui.height / 2,
        points=ui.getPoints()
    )

    contour = API.callApi(ImageArray)
    
    ui.displayImageWithContour(ImageArray, contour)

    params.saveParamsAsJson(ui.alphaUserInput.val, ui.betaUserInput.val, ui.getItrerations(), ui.getPoints())
main()

