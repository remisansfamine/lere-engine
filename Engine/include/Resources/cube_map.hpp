#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>

#include "texture.hpp"

namespace Resources
{
	class CubeMap : public Resource
	{
	private:
		GLuint ID = -1;
		std::vector<std::string> paths;
		CubeMapTexture textures[6];

	public:
		CubeMap(const std::vector<std::string>& paths);
		~CubeMap();

		GLuint	getID() const;
		bool generateBuffers();
		bool generateID();

		void bind() const;

		void mainThreadInitialization() override;
	};
}