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

		std::vector<std::string> skyPaths;

	public:
		SkyBox(Engine::Entity& owner, const std::vector<std::string>& paths);
		~SkyBox();

		void draw() const;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);

		void sendToProgram(std::shared_ptr<Resources::ShaderProgram> program) const;
	};
}