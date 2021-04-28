#include "renderer.hpp"

#include "render_manager.hpp"

namespace LowRenderer
{
	Renderer::Renderer(Engine::GameObject& gameObject, const std::shared_ptr<Renderer>& childPtr)
		: Component(gameObject, childPtr)
	{
		RenderManager::addRenderer(childPtr);
	}
}