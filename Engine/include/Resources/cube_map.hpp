#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>

#include "texture.hpp"

namespace Resources
{
	class CubeMap
	{
	private:
		GLuint ID = -1;

	public:
		CubeMap(const std::vector<std::string>& paths);
		~CubeMap();

		GLuint getID() const;

		void bind() const;
	};
}