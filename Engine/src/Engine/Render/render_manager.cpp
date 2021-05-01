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
		lights.clear();

		Core::Debug::Log::info("Destroying the Render Manager");
	}

	void RenderManager::draw()
	{
		RenderManager* RM = instance();

		for (std::shared_ptr<Renderer>& renderer : RM->renderers)
			renderer->draw();
	}

	void RenderManager::linkComponent(const std::shared_ptr<Light>& compToLink)
	{
		instance()->lights.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<Renderer>& compToLink)
	{
		instance()->renderers.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<Camera>& compToLink)
	{
		instance()->cameras.push_back(compToLink);
	}

	std::shared_ptr<Camera> RenderManager::getCurrentCamera()
	{
		RenderManager* RM = instance();

		if (RM->cameras.size() > 0)
			return RM->cameras.back();

		return nullptr;
	}
}