#pragma once

#include <string>

#include <glad/glad.h>

#include "resource.hpp"

namespace Resources
{
	class Texture : public Resource
	{
	private:
		GLuint textureID = 0;

		void generateID(int width, int height, float* colorBuffer);

	public:
		Texture(const std::string& filePath);

		GLuint getID();
	};
}
