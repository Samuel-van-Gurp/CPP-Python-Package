from setuptools import setup, find_packages, Distribution

class BinaryDistribution(Distribution):
    def has_ext_modules(self):
        return True


setup(
    name="SnakesPy",
    version="0.1.2",
    packages=["SnakesPy"],
    include_package_data=True,
    package_data={"SnakesPy": ["pybindings*.pyd", "params.json"]},
    distclass=BinaryDistribution,
)