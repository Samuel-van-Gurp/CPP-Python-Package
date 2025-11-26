from PIL import Image as PILImage
import numpy as np

class Image:
    def __init__(self, image_path):
        self.image_path = image_path
        self.image = None
        self.__load_image()

    def __load_image(self):
        try:
            img = PILImage.open(self.image_path)
            img = img.convert("L")  #to grayscale
            self.image = img
        except Exception as e:
            print(f"Failed to load image from {self.image_path}: {e}")
            self.image = None

    def downsampleImage(self, factor):
        if self.image is None:
            print("No image to downsample")
            return
        new_size = (self.image.width // factor, self.image.height // factor)
        self.image = self.image.resize(new_size, PILImage.Resampling.LANCZOS)
        print(f"Downsampled image to size: {self.image.size}")

    def show_image(self, output_path=None):
        if self.image is None:
            print("No image to show")
            return
        if output_path:
            try:
                self.image.save(output_path)
                print(f"Saved image to {output_path}")
            except Exception as e:
                print(f"Failed to save image: {e}")
        else:
            # Open the image in the default image viewer
            self.image.show()

    def convert_to_numpy(self):
        if self.image is None:
            print("No image to convert to numpy array")
            return None
        return np.array(self.image)
