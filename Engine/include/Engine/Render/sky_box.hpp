#pragma once

#include "component.hpp"
#include "shader.hpp"
#include "cube_map.hpp"
#include "mesh.hpp"

namespace LowRenderer
{
	class SkyBox : public Engine::Component
	{
	private:
		std::shared_ptr<Resources::CubeMap> cubeMap;
		std::shared_ptr<Resources::Mesh> cubeMesh;
		std::shared_ptr<Resources::ShaderProgram> m_shaderProgram = nullptr;

	public:
		SkyBox(Engine::GameObject& gameObject, const std::vector<std::string>& paths, const std::string& shaderPromgramName);
		SkyBox(Engine::GameObject& gameObject, const std::string& shaderPromgramName, std::shared_ptr<SkyBox> ptr);
		~SkyBox();

		void draw();
	};
}