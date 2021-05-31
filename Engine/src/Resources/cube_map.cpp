#include "cube_map.hpp"

#include "stb_image.h"

#include "debug.hpp"

namespace Resources
{
	CubeMap::CubeMap(const std::vector<std::string>& paths)
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		stbi_set_flip_vertically_on_load(false);

		int width, height, channel;
		for (unsigned int i = 0; i < 6; i++)
		{
			unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &channel, 0);
			if (data)
			{
				Core::Debug::Log::info("Loading of " + paths[i] + " done with success");
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			else
				Core::Debug::Log::error("Cannot find the texture file at " + paths[i]);

			stbi_image_free(data);
		}
		stbi_set_flip_vertically_on_load(true);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	CubeMap::~CubeMap()
	{
		glDeleteTextures(1, &ID);
	}

	GLuint CubeMap::getID() const
	{
		return ID;
	}

	void CubeMap::bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	}
}