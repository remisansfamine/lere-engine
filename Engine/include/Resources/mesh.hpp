#pragma once

#include <vector>
#include <string>

#include <glad\glad.h>

#include "maths.hpp"

#include "resource.hpp"

namespace Resources
{
	struct Vertex
	{
		Core::Maths::vec3 position;
		Core::Maths::vec3 texCoords;
		Core::Maths::vec3 tangent;
		Core::Maths::vec3 bitangent;
		Core::Maths::vec3 normal;
	};

	class Mesh : public Resource
	{
	private:
		GLuint VAO = 0;
		GLuint VBO = 0;

		void mainThreadInitialization() override;


	public:
		std::string parentMeshName;
		Mesh(const std::string& name, const std::string& parentMeshName);
		~Mesh();

		//std::vector<float> attributs;
		std::vector<Vertex> vertices;

		void parse(const std::string& toParse, std::array<unsigned int, 3> offsets);

		void draw() const;
		void generateVAO();
		void compute(std::array<unsigned int, 3> offsets, std::vector<Core::Maths::vec3>& vertices, std::vector<Core::Maths::vec3>& texCoords, std::vector<Core::Maths::vec3>& normals, std::vector<unsigned int>& indices);
	};
}
