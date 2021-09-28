#include "texture.hpp"

#include <imgui.h>

#include <chrono>
#include <filesystem>

#include "stb_image.h"

#include "debug.hpp"
#include "thread_pool.hpp"
#include "resources_manager.hpp"

#include "utils.hpp"

#include <cerrno>

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
		if (colorBuffer)
			stbi_image_free(colorBuffer);

		glDeleteTextures(1, &textureID);
		glActiveTexture(0);
	}

	bool Texture::generateBuffer()
	{
		if (stbiLoaded)
			return true;

		Core::Debug::Log::info("Start loading " + m_filePath + '.');

		int channel = 0;

		auto loadStart = std::chrono::system_clock::now();

		stbi_set_flip_vertically_on_load_thread(true);

		// Get the color buffer by using stbi
		colorBuffer = stbi_loadf(m_filePath.c_str(), &width, &height, &channel, STBI_rgb_alpha);

		stbi_set_flip_vertically_on_load_thread(false);
		
		auto loadEnd = std::chrono::system_clock::now();

		if (!colorBuffer)
		{
			std::string error = std::system_error(errno, std::system_category()).code().message();

			auto path = std::filesystem::current_path(); //getting path
			Core::Debug::Log::error("Cannot find the texture file at " + m_filePath + " in the directory " + path.generic_string() + ": " + error);
			return false;
		}

		std::chrono::duration<double> loadDuration = (loadEnd - loadStart) * 1000;

		std::string timeAsString = std::to_string(loadDuration.count());

		Core::Debug::Log::info("Loading of " + m_filePath + " done with success in " + timeAsString + " ms.");

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

		auto initStart = std::chrono::system_clock::now();

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
			stbi_image_free(colorBuffer);

		colorBuffer = nullptr;

		auto initEnd = std::chrono::system_clock::now();

		std::chrono::duration<double> initDuration = (initEnd - initStart) * 1000;

		std::string timeAsString = std::to_string(initDuration.count());

		Core::Debug::Log::info("OpenGL initialization of " + m_filePath + " done with success in " + timeAsString + " ms.");

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

	int Texture::getHeight() const
	{
		return height;
	}

	int Texture::getWidth() const
	{
		return width;
	}

	void Texture::drawImGui()
	{
		ImGui::Text(m_filePath.c_str());
		ImGui::Image((void*)(std::intptr_t)textureID, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
	}

	bool Texture::bind(int textureIndex) const
	{
		if (!textureID)
			return false;

		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, textureID);

		return true;
	}

	CubeMapTexture::CubeMapTexture(int ID, const std::string& filePath)
		: Texture(filePath), cubeMapID(ID)
	{

	}

	bool CubeMapTexture::generateBuffer()
	{
		int channel;
		stbi_set_flip_vertically_on_load_thread(false);
		colorBuffer = stbi_loadf(m_filePath.c_str(), &width, &height, &channel, 0);
		stbi_set_flip_vertically_on_load_thread(true);

		if (!colorBuffer)
		{
			std::string error = std::system_error(errno, std::system_category()).code().message();

			auto path = std::filesystem::current_path(); //getting path
			Core::Debug::Log::error("Cannot find the texture file at " + m_filePath + " in the directory " + path.generic_string() + ": " + error);
			return false;
		}

		Core::Debug::Log::info("Loading of " + m_filePath + " done with success");

		stbiLoaded = true;

		return colorBuffer;
	}

	bool CubeMapTexture::generateID()
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeMapID, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, colorBuffer);

		if (stbiLoaded)
			stbi_image_free(colorBuffer);

		colorBuffer = nullptr;

		return true;
	}
}