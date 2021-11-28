#include "renderer.hpp"

#include "render_manager.hpp"
#include "resources_manager.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	Renderer::Renderer(Engine::Entity& owner, const std::string& shaderProgramName, bool link)
		: Component(owner), m_shaderProgram(Resources::ResourcesManager::loadShaderProgram(shaderProgramName))
	{
		m_transform = requireComponent<Physics::TransformComponent>();
	}

	std::shared_ptr<Resources::ShaderProgram> Renderer::getProgram() const
	{
		return m_shaderProgram;
	}
}