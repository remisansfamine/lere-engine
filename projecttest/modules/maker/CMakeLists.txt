#cmake version
cmake_minimum_required(VERSION 3.16)

#project name
project("modules")

#c++ standard
set(CMAKE_CXX_STANDARD 14)

#project files macro
set(HPP_INCLUDE "${PROJECT_SOURCE_DIR}/include")
set(CPP_SRC "${PROJECT_SOURCE_DIR}/src")

#pybind11
add_subdirectory(externals/pybind11)

#module
pybind11_add_module(
	modules
	${HPP_INCLUDE}/modules.hpp
	${CPP_SRC}/modules.cpp
	${CPP_SRC}/binding.cpp
)

#include files for visual studio properties
target_include_directories(modules PRIVATE ${HPP_INCLUDE})