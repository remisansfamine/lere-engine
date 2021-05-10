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
		models.clear();
		lights.clear();
		sprites.clear();

		Core::Debug::Log::info("Destroying the Render Manager");
	}

	void RenderManager::drawModels()
	{
		std::shared_ptr<Resources::ShaderProgram> program;

		// TODO : sort renderers by shader
		if (skyBoxes.size() > 0)
		{
			skyBoxes.back()->draw();
		}

		// Number of lights to render (8 max)
		int lightCount = std::min((int)lights.size(), 8);

		for (int i = 0; i < lightCount; i++)
			lights[i]->compute();

		// Draw renderers
		for (std::shared_ptr<ModelRenderer>& model : models)
		{
			// If the ShaderProgram has changed, bind it and send shared informations
			if (program != model->getProgram())
			{
				program = model->getProgram();

				program->bind();

				getCurrentCamera()->sendViewProjToProgram(program);

				for (int i = 0; i < lightCount; i++)
					lights[i]->sendToProgram(program, i);
			}

			model->draw();
		}

		program->unbind();

		drawColliders();
	}

	void RenderManager::drawSprites()
	{
		std::shared_ptr<Resources::ShaderProgram> program;

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw renderers
		for (std::shared_ptr<SpriteRenderer>& sprite : sprites)
		{
			// If the ShaderProgram has changed, bind it and send shared informations
			if (program != sprite->getProgram())
			{
				program = sprite->getProgram();

				program->bind();

				getCurrentCamera()->sendViewOrthoToProgram(program);
			}

			sprite->draw();
		}

		program->unbind();
	}

	void RenderManager::draw()
	{
		RenderManager* RM = instance();

		RM->drawModels();
		RM->drawSprites();
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

		glEnable(GL_DEPTH_TEST);
	}

	void RenderManager::linkComponent(const std::shared_ptr<Light>& compToLink)
	{
		// Insert light to render
		instance()->lights.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<ModelRenderer>& compToLink)
	{
		// TODO: Insert with sorting
		instance()->models.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<SpriteRenderer>& compToLink)
	{
		// TODO: Insert with sorting
		instance()->sprites.push_back(compToLink);
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