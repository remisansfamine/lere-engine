#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "modules.hpp"

PYBIND11_MODULE(modules, handle)
{
	handle.doc() = "binding module";

	//function
	handle.def("add", &add);
	handle.def("sub", &sub);

	//class
	py::class_<Animal>(handle, "Animal")
		.def_readwrite("a", &Animal::a)
		.def_readwrite("b", &Animal::b)

		.def(py::init<>())
		.def("makeNull", &Animal::makeNull)
		.def("set", &Animal::set);
}