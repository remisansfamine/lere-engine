#include "cube_map.hpp"

#include "stb_image.h"

#include "debug.hpp"
#include "resources_manager.hpp"

namespace Resources
{
	CubeMap::CubeMap(const std::vector<std::string>& paths)
		: paths(paths)
	{
		for (int i = 0; i < 6; i++)
			textures[i] = CubeMapTexture(i, paths[i]);
	}

	CubeMap::~CubeMap()
	{
		glDeleteTextures(1, &ID);
	}

	bool CubeMap::generateBuffers()
	{
		stbi_set_flip_vertically_on_load(false);

		for (int i = 0; i < 6; i++)
			textures[i].generateBuffer();

		stbi_set_flip_vertically_on_load(true);

		ResourcesManager::addToMainThreadInitializerQueue(this);

		return true;
	}

	bool CubeMap::generateID()
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

		for (int i = 0; i < 6; i++)
			textures[i].generateID();

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return true;
	}

	GLuint CubeMap::getID() const
	{
		return ID;
	}

	void CubeMap::bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	}

	void CubeMap::mainThreadInitialization()
	{
		generateID();
	}
}