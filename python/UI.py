from matplotlib.patches import Ellipse
import matplotlib.pyplot as plt
from matplotlib.widgets import Button, EllipseSelector, PolygonSelector
import numpy as np


class UI:
    def __init__(self):
        self.selector = None
        self.ellipse_patch = None 

    def displayImageWithContour(self, image: np.ndarray, contour: np.ndarray):
        plt.imshow(image, cmap='gray')
        plt.plot(contour[:, 0], contour[:, 1], color='red') 
        plt.title('Image with Contour Overlay')
        plt.axis('off')
        plt.show()

    def selectInitContour(self, image: np.ndarray):

        fig, ax = plt.subplots()
        ax.imshow(image, cmap='gray')
        ax.set_title('Select initial ellipse')


        self.selector = EllipseSelector(ax, self.onSelect)
        button_ax = fig.add_axes([0.4, 0.05, 0.2, 0.075])
        btn = Button(button_ax, 'My Button')
        btn.on_clicked(self.on_button_clicked)
        # plt.axis('off')
        plt.show()  # This keeps the window open until closed by the user

    def on_button_clicked(self, event):
        print("button clicked,")
        xy = self.ellipse_patch.xy()
        width = self.ellipse_patch.width()
        height = self.ellipse_patch.height()

    def onSelect(self, eclick, erelease):
        ax = eclick.inaxes
        
        if self.ellipse_patch is not None:
            self.ellipse_patch.remove()
            self.ellipse_patch = None

        # draw the ellipse on the image
        ax = eclick.inaxes
        ellipse = Ellipse(
                            xy=((eclick.xdata + erelease.xdata) / 2, (eclick.ydata + erelease.ydata) / 2),
                            width=abs(erelease.xdata - eclick.xdata),
                            height=abs(erelease.ydata - eclick.ydata),
                            edgecolor='red',
                            facecolor='none'
                         )
        ax.add_patch(ellipse)
        self.ellipse_patch = ellipse
        plt.draw()