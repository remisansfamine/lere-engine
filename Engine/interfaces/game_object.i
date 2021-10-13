%module game_object

%{
#include "../include/Engine/game_object.hpp"

using namespace Engine;
%}

%include object.i
%include ../include/Engine/game_object.hpp