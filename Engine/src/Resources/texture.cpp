#include "texture.hpp"

#include <imgui.h>

#include <chrono>

#include "stb_image.h"

#include "debug.hpp"
#include "thread_pool.hpp"
#include "resources_manager.hpp"

#include "utils.hpp"

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
	}

	Texture::Texture(const std::string& name, int width, int height, float* colorBuffer)
		: Resource(name), width(width), height(height), colorBuffer(colorBuffer)
	{
		ResourcesManager::addToMainThreadInitializerQueue(this);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &textureID);
	}

	bool Texture::generateBuffer(const std::string& filePath)
	{
		if (stbiLoaded)
			return true;

		auto loadStart = std::chrono::system_clock::now();

		Core::Debug::Log::info("Start loading " + filePath + '.');

		stbi_set_flip_vertically_on_load(true);

		int channel = 0;

		// Get the color buffer by using stbi
		colorBuffer = stbi_loadf(filePath.c_str(), &width, &height, &channel, STBI_rgb_alpha);

		if (!colorBuffer)
		{
			Core::Debug::Log::error("Cannot find the texture file at " + filePath);
			return false;
		}

		auto loadEnd = std::chrono::system_clock::now();

		std::chrono::duration<double> loadDuration = (loadEnd - loadStart) * 1000;

		std::string timeAsString = std::to_string(loadDuration.count());

		Core::Debug::Log::info("Loading of " + filePath + " done with success in " + timeAsString + " ms.");

		stbiLoaded = true;

		ResourcesManager::addToMainThreadInitializerQueue(this);

		return true;
	}

	bool Texture::generateID()
	{
		if (!colorBuffer || textureID)
		{
			Core::Debug::Log::error("Texture at " + m_filePath + " is already OpenGL initialized");
			return false;
		}

		Core::Debug::Log::info("OpenGL initializing texture at " + m_filePath);

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

		// Free the color buffer allocated by stbi
		if (stbiLoaded)
		{
			stbi_image_free(colorBuffer);
		}

		colorBuffer = nullptr;

		Core::Debug::Log::info("OpenGL initializion of " + m_filePath + " done with succes.");

		return true;
	}

	void Texture::mainThreadInitialization()
	{
		generateID();
	}

	GLuint Texture::getID() const
	{
		return textureID;
	}

	bool Texture::bind(int textureIndex) const
	{
		if (!textureID)
		{
			//generateID(width, height, colorBuffer);
			return false;
		}

		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, textureID);

		return true;
	}
}