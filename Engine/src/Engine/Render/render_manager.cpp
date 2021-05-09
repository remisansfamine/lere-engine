#include "render_manager.hpp"

#include <algorithm>

#include "debug.hpp"
#include "resources_manager.hpp"

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
		if (RM->skyBoxes.size() > 0)
		{
			RM->skyBoxes.back()->draw();
		}

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

				getCurrentCamera()->sendViewProjToProgram(program);

				for (int i = 0; i < lightCount; i++)
					RM->lights[i]->sendToProgram(program, i);
			}

			renderer->draw();
		}

		program->unbind();

		RM->drawColliders();
	}

	void RenderManager::drawColliders() const
	{
		if (colliders.size() == 0)
			return;

		glDisable(GL_DEPTH_TEST);

		std::shared_ptr<Resources::ShaderProgram> program = colliders[0]->getProgram();

		program->bind();

		getCurrentCamera()->sendProjToProgram(program);

		for (const std::shared_ptr<ColliderRenderer>& rendererCollider : colliders)
		{
			if (rendererCollider->canBeDraw())
				rendererCollider->draw();
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

	void RenderManager::linkComponent(const std::shared_ptr<SkyBox>& compToLink)
	{
		// Insert camera to render
		instance()->skyBoxes.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<ColliderRenderer>& compToLink)
	{
		// Insert camera to render
		instance()->colliders.push_back(compToLink);
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