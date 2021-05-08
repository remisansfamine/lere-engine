#include "texture.hpp"

#include "stb_image.h"

#include "debug.hpp"

namespace Resources
{
	 std::shared_ptr<Texture> Texture::defaultAlpha = nullptr;
	 std::shared_ptr<Texture> Texture::defaultAmbient = nullptr;
	 std::shared_ptr<Texture> Texture::defaultDiffuse = nullptr;
	 std::shared_ptr<Texture> Texture::defaultEmissive = nullptr;
	 std::shared_ptr<Texture> Texture::defaultSpecular = nullptr;


	Texture::Texture(const std::string& filePath)
		: Resource(filePath)
	{
		stbi_set_flip_vertically_on_load(true);

		int width = 0, height = 0;
		int channel = 0;

		// Get the color buffer by using stbi
		float* colorBuffer = stbi_loadf(filePath.c_str(), &width, &height, &channel, STBI_rgb_alpha);

		if (colorBuffer)
		{
			Core::Debug::Log::info("Loading of " + filePath + " done with success");

			generateID(width, height, colorBuffer);

			// Free the color buffer allocated by stbi
			stbi_image_free(colorBuffer);
		}
		else
			Core::Debug::Log::error("Cannot find the texture file at " + filePath);
	}

	Texture::Texture(int width, int height, float* colorBuffer)
	{
		generateID(width, height, colorBuffer);
	}

	void Texture::generateID(int width, int height, float* colorBuffer)
	{
		// Set the texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Generate the texture ID
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, colorBuffer);

		// Generate its mipmap
		glGenerateMipmap(GL_TEXTURE_2D);

		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLuint Texture::getID() const
	{
		return textureID;
	}

	void Texture::bind(int textureIndex) const
	{
		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
}