#pragma once

#include <memory>
#include <vector>

#include "mesh.hpp"
#include "shader.hpp"

namespace LowRenderer
{
	class Model
	{
	private:
		std::shared_ptr<Resources::Mesh> mesh = nullptr;
		std::shared_ptr<Resources::ShaderProgram> shaderProgram = nullptr;
		//std::shared_ptr<Resources::Material> material = nullptr;

		std::vector<Model> children;

	public:
	};
}
