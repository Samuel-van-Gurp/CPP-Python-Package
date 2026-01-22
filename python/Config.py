import os
from pathlib import Path

# path to the C++ pybindings build directory
BASE_DIR = Path(__file__).resolve().parent

PYBINDINGS_DIR = BASE_DIR.parent / "build" / "Debug"

PARAMETER_JSON_PATH = BASE_DIR / "Params" / "params.json"