from .ActiveContour import ActiveContour

def main():
    active_contour = ActiveContour("Images/astronaut.png")
    active_contour.run()

if __name__ == "__main__":
    main()