#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Core::Input
{
	class Input
	{
	public:
		virtual void compute() = 0;

		static GLFWwindow* window;
	};
}