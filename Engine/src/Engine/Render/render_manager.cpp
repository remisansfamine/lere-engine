#include "render_manager.hpp"

#include "debug.hpp"

namespace LowRenderer
{
	RenderManager::RenderManager()
	{
		Core::Debug::Log::info("Creating the Render Manager");
	}

	RenderManager::~RenderManager()
	{
		renderers.clear();

		Core::Debug::Log::info("Destroying the Render Manager");
	}

	void RenderManager::draw()
	{
		RenderManager* RM = instance();

		for (std::shared_ptr<Renderer> renderer : RM->renderers)
			renderer->draw();
	}

	void RenderManager::addRenderer(const std::shared_ptr<Renderer>& rendererToDraw)
	{
		// TODO
		instance()->renderers.push_back(rendererToDraw);
	}

	void RenderManager::clearRenderers()
	{
		instance()->renderers.clear();
	}
}