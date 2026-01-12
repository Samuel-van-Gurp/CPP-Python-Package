import Params
import Image
import PyAPI
from DataClasses.DataClasses import SnakeParams, ImageInfo
from UI import UI
import json

def main():
    # create instances
    ui = UI()
    pyAPI = PyAPI.PyAPI(SnakeParams(), ImageInfo())
    params = Params.Params()

    # load image
    CoinImage = Image.Image("Images/coin.jpg")
    ImageArray = CoinImage.convert_to_numpy()
    # load params from json
    pyAPI.setSnakeParams(alpha=params.alpha, 
                        beta=params.beta,
                        iterations=params.max_iterations,
                        center_x=0.0,
                        center_y=0.0,
                        radius_x=0.0,
                        radius_y=0.0,
                        points=params.contour_points
                        )

    ui.selectInitContour(ImageArray, alpha_init=params.alpha, beta_init=params.beta)
    
    pyAPI.setImageInfo(ImageArray)
    pyAPI.setSnakeParams(alpha = ui.alphaUserInput.val, 
                        beta = ui.betaUserInput.val, 
                        iterations=100, 
                        # center_x=130, 
                        # center_y=100, 
                        # radius_x=40, 
                        # radius_y=40, 
                        center_x=ui.xy[0], 
                        center_y=ui.xy[1], 
                        radius_x=ui.width / 2, 
                        radius_y=ui.height / 2, 
                        points=100)

    contour = pyAPI.callApi(ImageArray)

    print(contour)

    ui.displayImageWithContour(ImageArray, contour)

    params.saveParamsAsJson(ui.alphaUserInput.val, ui.betaUserInput.val, 500, 50)

main()

