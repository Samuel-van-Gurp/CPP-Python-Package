from pathlib import Path
import argparse
from .ActiveContour import ActiveContour

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("image", nargs="?", default=str(Path(__file__).resolve().parent.parent / "Images" / "astronaut.png")) # default for developnet experice
    args = parser.parse_args()

    activeContour = ActiveContour(args.image) 
    activeContour.run()
if __name__ == "__main__":
    main()