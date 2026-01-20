#include <pybind11/pybind11.h>
#include "api_impl.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pyapi, m) {
    m.doc() = "pybind11 bindings for C++ API";

    m.def("add", [](int a, int b){ return a + b; }, "Add two ints");

    // example: expose an existing function
    // m.def("compute", &cpp_api::compute, "Compute using C++ function");
}