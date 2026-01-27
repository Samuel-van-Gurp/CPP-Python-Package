#include <vector>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "DataObjects/SnakeParams.hpp"
#include "DataObjects/ImageInfo.hpp"
#include "DataObjects/Point.hpp"
#include "SnakeController.hpp" 

namespace py = pybind11;
    
std::vector<Point> RunSnakeAPI(py::array_t<float> inputImage, const SnakeParams& params, SnakeSolver solver = SnakeSolver::EULER_LAGRANGE) {

    // check dimensions
        try {
        if (inputImage.ndim() != 2) {
            throw std::runtime_error("Input image must be a 2D array");
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
        py::buffer_info buf = inputImage.request(); // request metadata of the array
    float* ptr = static_cast<float*>(buf.ptr);  // get the pointer to the first element in the array
    size_t size = buf.shape[0] * buf.shape[1];  // assuming 2D array
    std::vector<float> vec(ptr, ptr + size);    // copy data into a std::vector

    // Construct ImageInfo
    ImageInfo imageInfo;
    imageInfo.data = vec; 
    imageInfo.width = buf.shape[1];
    imageInfo.height = buf.shape[0];

    auto snakeController = SnakeController::createSnakeController(ImageHolder<float>(imageInfo.data, imageInfo.width, imageInfo.height), params, solver);
    auto contour = snakeController.run(params.iterations);

    return contour;
}
 
PYBIND11_MODULE(pybindings, m) {
    m.doc() = "pybind11 bindings for C++ API"; // TODO, i saw you can link this to the README.md

    // expose the snakeParams struct to Python
    py::class_<SnakeParams>(m, "SnakeParams")
        .def(py::init<>())
        .def_readwrite("alpha", &SnakeParams::alpha)
        .def_readwrite("beta", &SnakeParams::beta)
        .def_readwrite("iterations", &SnakeParams::iterations)
        .def_readwrite("contour_center_x", &SnakeParams::contour_center_x)
        .def_readwrite("contour_center_y", &SnakeParams::contour_center_y)
        .def_readwrite("contour_radius_x", &SnakeParams::contour_radius_x)
        .def_readwrite("contour_radius_y", &SnakeParams::contour_radius_y)
        .def_readwrite("contour_points", &SnakeParams::contour_points);

    // expose the Point struct to Python
    py::class_<Point>(m, "Point")
        .def(py::init<>())
        .def_readwrite("x", &Point::X)
        .def_readwrite("y", &Point::Y); 

    // expose the solver setting enum to Python
    py::enum_<SnakeSolver>(m, "SnakeSolver")
        .value("GREEDY", SnakeSolver::GREEDY_ALGORITHM)
        .value("EULER_LAGRANGE", SnakeSolver::EULER_LAGRANGE)
        .export_values();
    
    m.def("RunSnake", &RunSnakeAPI, "Run the snake algorithm",
          py::arg("inputImage"), py::arg("params"), py::arg("solver") = SnakeSolver::EULER_LAGRANGE);
}

