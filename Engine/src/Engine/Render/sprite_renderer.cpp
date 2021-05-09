#include "sprite_renderer.hpp"

#include "render_manager.hpp"
#include "inputs_manager.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	SpriteRenderer::SpriteRenderer(Engine::GameObject& gameObject, const std::string& shaderPromgramName)
		: Renderer(gameObject, std::shared_ptr<SpriteRenderer>(this), shaderPromgramName)
	{ 
	}

	void SpriteRenderer::draw() const
	{
	}

	void SpriteRenderer::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		/*std::string modelPath, shaderProgramName;

		iss >> modelPath;
		iss >> shaderProgramName;

		gameObject.addComponent<SpriteRenderer>(modelPath, shaderProgramName);*/
	}
}