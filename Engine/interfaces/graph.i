%module graph

%{
#include "../wrappers/graph_wrapper.hpp"

using namespace Scripting::Graph;
%}

%include std_string.i
%include ../wrappers/graph_wrapper.hpp