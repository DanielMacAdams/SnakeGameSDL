#include <pybind11/pybind11.h>
#include "snake_game.h"
namespace py = pybind11;

PYBIND11_MODULE(snake_game, m){
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("multiply", &multiply, "A function that multiplies two numbers",
        pybind11::arg("a"), pybind11::arg("b"));
}