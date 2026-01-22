import Params
import Image
import PyAPI
import Config
import pybindings
from UI import UI

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
        iterations=500,
        center_x=ui.xy[0],
        center_y=ui.xy[1],
        radius_x=ui.width / 2,
        radius_y=ui.height / 2,
        points=50
    )

    contour = API.callApi(ImageArray)
    
    ui.displayImageWithContour(ImageArray, contour)

    params.saveParamsAsJson(ui.alphaUserInput.val, ui.betaUserInput.val, 500, 50)

main()

