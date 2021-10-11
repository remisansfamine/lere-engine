%module debug

%{
#include "../wrappers/debug_wrapper.hpp"

using namespace Scripting::Debug;
%}

%include std_string.i
%include ../wrappers/debug_wrapper.hpp