%module engine

%{
#include "../include/Core/maths.hpp"
#include "../include/Engine/Physics/transform.hpp"

using namespace Core::Maths;
using namespace Physics;
%}

namespace Core::Maths
{
	union vec3
	{
		float x, y, z;
	};
}

%include ../include/Engine/Physics/transform.hpp