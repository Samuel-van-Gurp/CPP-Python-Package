from matplotlib.patches import Ellipse
import matplotlib.pyplot as plt
from matplotlib.widgets import Button, EllipseSelector, Slider
import numpy as np


class UI:
    def __init__(self):
        self.selector = None
        self.ellipse_patch = None 
        self.xy = None
        self.width = None
        self.height = None
        self.alphaUserInput = None
        self.betaUserInput = None
        self.fig = None
        self.ax = None
        self.result_contour = None 
        

    def setResultContour(self, contour : np.ndarray):
        self.result_contour = contour


    def selectInitContour(self, image: np.ndarray):
        self.fig, self.ax = plt.subplots()
        ax = self.ax
        fig = self.fig
        ax.imshow(image, cmap='gray')
        ax.set_title('Select initial ellipse')
        ax.set_xticks([])      
        ax.set_yticks([])      
        ax.set_axis_off()   


        self.selector = EllipseSelector(ax, self.onSelect)
        button_ax = fig.add_axes([0.4, 0.05, 0.2, 0.075])
        slider_alpha_ax = fig.add_axes([0.1, 0.05, 0.2, 0.075])
        slider_beta_ax = fig.add_axes([0.1, 0.1, 0.2, 0.075])
        self.alphaUserInput = Slider(slider_alpha_ax, 'α', valmin=0, valmax=1, valinit=0.5)
        self.betaUserInput = Slider(slider_beta_ax, 'β', valmin=0, valmax=1, valinit=0.5)
        btn = Button(button_ax, 'Confirm Selection')
        btn.on_clicked(self.onButtonClicked)

        plt.show()

    def onButtonClicked(self, event):
        print("onButtonClicked::self.ellipse_patch.get_center())",self.ellipse_patch.get_center())

        if self.ellipse_patch is not None:
            self.xy = self.ellipse_patch.get_center()
            self.width = self.ellipse_patch.get_width() 
            self.height = self.ellipse_patch.get_height()

            # self.ellipse_patch = None
        else:
            print("No ellipse selected!")

        plt.close(self.fig)

    def displayImageWithContour(self, image : np.ndarray, contour: np.ndarray):
        plt.imshow(image, cmap='gray')
        plt.plot(contour[:, 0], contour[:, 1], color='red')
        plt.title('Final Snake Result')
        plt.axis('off')
        plt.draw()
        plt.show()

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
        print("self.ellipse_patch.get_center())",self.ellipse_patch.get_center())
        print("onselect")