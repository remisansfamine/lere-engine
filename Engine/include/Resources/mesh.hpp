#pragma once

#include <vector>
#include <string>

#include <glad\glad.h>

#include "maths.hpp"

namespace Resources
{
	class Mesh
	{
	private:

		GLuint VAO;


	public:
		std::vector<float> attributs;
		std::string name;

		void draw() const;
		void generateVAO();
		void compute(std::vector<Core::Maths::vec3>& vertices, std::vector<Core::Maths::vec3>& texCoords, std::vector<Core::Maths::vec3>& normals, std::vector<unsigned int>& indices);
	};
}
