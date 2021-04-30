#pragma once

#include <vector>
#include <string>

#include <glad\glad.h>

namespace Resources
{
	class Mesh
	{
	private:
		std::vector<float> attributs;

		GLuint VAO;
		std::string name;
	public:

	};
}
