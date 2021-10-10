#cmake version
cmake_minimum_required(VERSION 3.4)

#project name
project(modules)

#c++ standard
set(CMAKE_CXX_STANDARD 14)

#project files macro
set(INCLUDE "${PROJECT_SOURCE_DIR}/include")
set(SRC "${PROJECT_SOURCE_DIR}/src")

#pybind11
add_subdirectory(externals/pybind11)

#module
pybind11_add_module(modules INCLUDE/modules.hpp SRC/modules.cpp SRC/binding.cpp)

#include files for visual studio properties
target_include_directories(modules PRIVATE INCLUDE)