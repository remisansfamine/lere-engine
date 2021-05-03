#pragma once

#include "component.hpp"
#include "shader.hpp"

namespace Physics
{
	class Transform;
}

namespace LowRenderer
{
	class Renderer : public Engine::Component
	{
	protected:
		Renderer(Engine::GameObject& gameObject, const std::shared_ptr<Renderer>& childPtr, const std::string& shaderProgramName);

		std::shared_ptr<Physics::Transform> m_transform = nullptr;
		std::shared_ptr<Resources::ShaderProgram> m_shaderProgram = nullptr;

	public:

		void virtual draw() = 0;
		std::shared_ptr<Resources::ShaderProgram> getProgram();
	};
}