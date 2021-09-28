#include "render_manager.hpp"

#include <algorithm>

#include "debug.hpp"
#include "resources_manager.hpp"

#include "shader.hpp"
#include "shadow.hpp"


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

	void RenderManager::drawShadows()
	{
		std::shared_ptr<Resources::ShaderProgram> program;

		glCullFace(GL_FRONT);

		// Number of lights to render (8 max)
		int lightCount = std::min((int)lights.size(), 8);

		for (int i = 0; i < lightCount; i++)
			lights[i]->compute();

		for (auto& light : lights)
		{
			if (!light->isActive() || light->shadow == nullptr)
				continue;

			program = light->shadow->program;

			program->bind();

			light->shadow->sendToShader(light);

			if (!light->shadow->bindAndSetViewport())
				continue;

			glClear(GL_DEPTH_BUFFER_BIT);

			for (auto& model : models)
				model->simpleDraw(program);

			light->shadow->unbindAndResetViewport();
		}
	}

	void RenderManager::drawSkybox()
	{
		glEnable(GL_FRAMEBUFFER_SRGB);

		if (skyBoxes.size() > 0)
		{
			if (skyBoxes.back()->isActive())
				skyBoxes.back()->draw();
		}
	}

	void RenderManager::drawModels()
	{
		std::shared_ptr<Resources::ShaderProgram> program;

		glCullFace(GL_BACK);

		// Number of lights to render (8 max)
		int lightCount = std::min((int)lights.size(), 8);

		// Draw renderers
		for (std::shared_ptr<ModelRenderer>& model : models)
		{
			if (!model->isActive())
				continue;

			// If the ShaderProgram has changed, bind it and send shared informations
			if (program != model->getProgram())
			{
				program = model->getProgram();

				if (!program->bind())
					continue;

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

		glClear(GL_DEPTH_BUFFER_BIT);

		// Draw renderers
		for (std::shared_ptr<SpriteRenderer>& sprite : sprites)
		{
			if (!sprite->isActive())
				continue;

			// If the ShaderProgram has changed, bind it and send shared informations
			if (program != sprite->getProgram())
			{
				program = sprite->getProgram();

				if (!program->bind())
					continue;

				getCurrentCamera()->sendViewOrthoToProgram(program);
			}

			sprite->draw();
		}

		program->unbind();

		glDisable(GL_FRAMEBUFFER_SRGB);
	}

	void RenderManager::draw()
	{
		RenderManager* RM = instance();

		// Draw Shadows
		RM->drawShadows();
		RM->drawSkybox();
		RM->drawModels();
		RM->drawSprites();
	}

	void RenderManager::drawColliders() const
	{
		if (colliders.size() == 0)
			return;

		glDisable(GL_DEPTH_TEST);

		std::shared_ptr<Resources::ShaderProgram> program = colliders[0]->getProgram();

		if (!program->bind())
			return;

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
		// Insert model to renderer
		instance()->models.push_back(compToLink);
	}

	void RenderManager::linkComponent(const std::shared_ptr<SpriteRenderer>& compToLink)
	{
		// Insert sprite to renderer
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

	void RenderManager::removeComponent(SpriteRenderer* compToRemove)
	{
		RenderManager* RM = instance();

		for (auto it = RM->sprites.begin(); it != RM->sprites.end(); it++)
		{
			if (it->get() == compToRemove)
			{
				RM->sprites.erase(it);
				break;
			}
		}
	}

	void RenderManager::removeComponent(ModelRenderer* compToRemove)
	{
		RenderManager* RM = instance();

		for (auto it = RM->models.begin(); it != RM->models.end(); it++)
		{
			if (it->get() == compToRemove)
			{
				RM->models.erase(it);
				break;
			}
		}
	}

	void RenderManager::clearAll()
	{
		clearComponents<LowRenderer::SpriteRenderer>();
		clearComponents<LowRenderer::ColliderRenderer>();
		clearComponents<LowRenderer::ModelRenderer>();
		clearComponents<LowRenderer::Camera>();
		clearComponents<LowRenderer::Light>();
		clearComponents<LowRenderer::SkyBox>();
	}

	std::shared_ptr<Camera> RenderManager::getCurrentCamera()
	{
		RenderManager* RM = instance();

		if (RM->cameras.size() > 0)
			return RM->cameras.back();

		return nullptr;
	}
}