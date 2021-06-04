#pragma once

#include <string>
#include <memory>

#include <glad/glad.h>

#include "resource.hpp"

#include "maths.hpp"

namespace Resources
{
	class Texture : public Resource
	{
	private:
		GLuint textureID = 0;
		
		void generateID(int width, int height, float* colorBuffer);

	public:
		Texture(const std::string& filePath);
		Texture(int width, int height, float* colorBuffer);
		~Texture();

		GLuint getID() const;

		void bind(int textureIndex) const;

		static std::shared_ptr<Texture> defaultAlpha;
		static std::shared_ptr<Texture> defaultAmbient;
		static std::shared_ptr<Texture> defaultDiffuse;
		static std::shared_ptr<Texture> defaultEmissive;
		static std::shared_ptr<Texture> defaultSpecular;
	};	
}
