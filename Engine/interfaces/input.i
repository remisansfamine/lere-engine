%module input

%{
#include "../wrappers/input_wrapper.hpp"

using namespace Scripting::Input;
%}

%include std_string.i
%include ../wrappers/input_wrapper.hpp