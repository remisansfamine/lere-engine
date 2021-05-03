#include "render_manager.hpp"

#include <algorithm>

#include "debug.hpp"

#include "shader.hpp"


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

		std::shared_ptr<Resources::ShaderProgram> program;

		// TODO : sort renderers by shader

		// Number of lights to render (8 max)
		int lightCount = std::min((int)RM->lights.size(), 8);

		for (int i = 0; i < lightCount; i++)
			RM->lights[i]->compute();

		// Draw renderers
		for (std::shared_ptr<Renderer>& renderer : RM->renderers)
		{
			// If the ShaderProgram has changed, bind it and send shared informations
			if (program != renderer->getProgram())
			{
				program = renderer->getProgram();

				program->bind();

				getCurrentCamera()->sendToProgram(program);

				for (int i = 0; i < lightCount; i++)
					RM->lights[i]->sendToProgram(program, i);
			}

			renderer->draw();
		}

		program->unbind();
	}

	void RenderManager::linkComponent(const std::shared_ptr<Light>& compToLink)
	{
		// Insert light to render
		instance()->lights.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<Renderer>& compToLink)
	{
		// TODO: Insert with sorting
		instance()->renderers.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<Camera>& compToLink)
	{
		// Insert camera to render
		instance()->cameras.push_back(compToLink);
	}

	std::shared_ptr<Camera> RenderManager::getCurrentCamera()
	{
		RenderManager* RM = instance();

		// TODO: Get a real current camera
		if (RM->cameras.size() > 0)
			return RM->cameras.back();

		return nullptr;
	}
}