from pathlib import Path
import os

# path to the C++ pybindings build directory
BASE_DIR = Path(__file__).resolve().parent

PYBINDINGS_DIR = "C:\\Users\\svangurp\\Desktop\\CPP_PY_package\\python_code\\SnakesPy\\pybindings.cp314-win_amd64.pyd" # BASE_DIR.parent.parent / "build" / "Debug"

# Look for params.json in the current working directory (where user's script is)
import json

PARAMETER_JSON_PATH = Path(os.getcwd()) / "params.json"