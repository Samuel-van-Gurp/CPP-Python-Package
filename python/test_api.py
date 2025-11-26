import queue
import threading
import Image
import PyAPI
from DataClasses.DataClasses import SnakeParams, ImageInfo
from UI import UI
import matplotlib.pyplot as plt

def main():
    CoinImage = Image.Image("StarImage.png")

    ImageArray = CoinImage.convert_to_numpy()

    ui = UI()

    pyAPI = PyAPI.PyAPI(SnakeParams(), ImageInfo())

    pyAPI.setImageInfo(ImageArray)
    
    ui.selectInitContour(ImageArray)
    
    pyAPI.setSnakeParams(alpha = ui.alphaUserInput.val, 
                        beta = ui.betaUserInput.val, 
                        iterations=500, 
                        center_x=ui.xy[0], 
                        center_y=ui.xy[1], 
                        radius_x=ui.width / 2, 
                        radius_y=ui.height / 2, 
                        points=50)

    contour = pyAPI.callApi(ImageArray)

    ui.displayImageWithContour(ImageArray, contour)

    

main()