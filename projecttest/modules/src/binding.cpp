#include <pybind11/pybind11.h>
#include "modules.hpp"

namespace py = pybind11;

PYBIND11_MODULE(modules, handle)
{
	handle.doc() = "the module doc";
	handle.def("add", &add);
}