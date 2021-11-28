#pragma once

#include "component.hpp"
#include "shader.hpp"

namespace Physics
{
	class TransformComponent;
}

namespace LowRenderer
{
	class Renderer : public Engine::Component
	{
	protected:
		Physics::TransformComponent* m_transform = nullptr;
		std::shared_ptr<Resources::ShaderProgram> m_shaderProgram = nullptr;

		Renderer(Engine::Entity& owner, const std::string& shaderProgramName, bool link = true);

	public:
		std::shared_ptr<Resources::ShaderProgram> getProgram() const;

		void virtual draw() const = 0;
	};
}