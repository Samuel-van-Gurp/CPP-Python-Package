import Params
import Image
import PyAPI
import pybindings
from DataClasses.DataClasses import SnakeParams, ImageInfo

from UI import UI
import json

def main():
    # create instances
    ui = UI()

    # load image
    CoinImage = Image.Image("Images/astronaut.png")
    ImageArray = CoinImage.convert_to_numpy()

    # load params from json
    params = Params.Params()
    params.loadparams_from_json("Params/params.json")

    # pyAPI = PyAPI_Pybind11(SnakeParams(alpha=params.alpha, 
    #                     beta=params.beta,
    #                     iterations=params.max_iterations,
    #                     center_x=0.0,
    #                     center_y=0.0,
    #                     radius_x=0.0,
    #                     radius_y=0.0,
    #                     points=params.contour_points)
    #                     )

    ui.selectInitContour(ImageArray, alpha_init=params.alpha, beta_init=params.beta)
    
    snake_params = pybindings.SnakeParams()
    snake_params.alpha = ui.alphaUserInput.val
    snake_params.beta = ui.betaUserInput.val
    snake_params.iterations = 100
    snake_params.contour_center_x = ui.xy[0]
    snake_params.contour_center_y = ui.xy[1]
    snake_params.contour_radius_x = ui.width / 2
    snake_params.contour_radius_y = ui.height / 2
    snake_params.contour_points = 100   

    pyAPI = PyAPI.PyAPI_Pybind11(snake_params)

    contour = pyAPI.callApi(ImageArray)

    # print("Contour from API:", contour)
    ui.displayImageWithContour(ImageArray, contour)

    params.saveParamsAsJson(ui.alphaUserInput.val, ui.betaUserInput.val, 500, 50)

main()

