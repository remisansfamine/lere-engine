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

		void bind(int textureIndex);

		//static std::shared_ptr<Texture> defaultAlpha;
		//static std::shared_ptr<Texture> defaultAmbient;
		//static std::shared_ptr<Texture> defaultDiffuse;
		//static std::shared_ptr<Texture> defaultEmissive;
		//static std::shared_ptr<Texture> defaultSpecular;
	};	
}
