#include "render_manager.hpp"

#include <algorithm>

#include "debug.hpp"
#include "resources_manager.hpp"

#include "shader.hpp"
#include "shadow.hpp"

#include "uniform.hpp"

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

	void RenderManager::GLSetCapState(const GLenum cap, bool state)
	{
		RenderManager* renderManager = instance();

		if (renderManager->enabledCaps[cap] == state)
			return;

		renderManager->enabledCaps[cap] = state;
		state ? glEnable(cap) : glDisable(cap);
	}


	void RenderManager::GLEnable(const GLenum cap)
	{
		GLSetCapState(cap, true);
	}

	void RenderManager::GLDisable(const GLenum cap)
	{
		GLSetCapState(cap, false);
	}

	void RenderManager::drawShadows()
	{
		std::shared_ptr<Resources::ShaderProgram> program;

		glCullFace(GL_FRONT);

		// Number of lights to render (8 max)
		int lightCount = std::min((int)lights.size(), 8);

		int i = 0;
		for (auto& light : lights)
		{
			if (i >= lightCount)
				break;

			light->compute();
			i++;
		}

		for (const auto& light : lights)
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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderManager::drawSkybox()
	{
		glCullFace(GL_FRONT);

		for (auto& skyBox : skyBoxes)
		{
			if (skyBox->isActive())
				skyBox->draw();
		}
	}

	void RenderManager::drawModels()
	{
		std::shared_ptr<Resources::ShaderProgram> program;

		GLEnable(GL_FRAMEBUFFER_SRGB);

		GLEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Number of lights to render (8 max)
		int lightCount = std::min((int)lights.size(), 8);

		// Draw renderers
		for (const auto& model : models)
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

				std::vector<LightData> lightDatas;

				int i = 0;
				for (auto& light : lights)
				{
					if (i >= lightCount)
						break;

					light->sendToProgram(program, i);

					light->addToLightBuffer(lightDatas);

					i++;
				}

				glActiveTexture(0);

				UniformBlock& lightBlock = uniformBlocks["lightBlock"];

				lightBlock.bind();
				//glBufferSubData(GL_UNIFORM_BUFFER, 0, lightDatas.size() * sizeof(LightData), lightDatas.data());
				lightBlock.unbind();

				for (auto& skyBox : skyBoxes)
					skyBox->sendToProgram(program);
			}

			model->draw();
		}

		program->unbind();

		drawColliders();
	}

	void RenderManager::drawSprites()
	{
		std::shared_ptr<Resources::ShaderProgram> program;

		glCullFace(GL_BACK);

		glClear(GL_DEPTH_BUFFER_BIT);

		// Draw renderers
		for (const auto& sprite : sprites)
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

		GLDisable(GL_FRAMEBUFFER_SRGB);
	}

	void RenderManager::draw()
	{
		RenderManager* RM = instance();

		RM->drawShadows();
		RM->drawModels();
		RM->drawSkybox();
		RM->drawSprites();
	}

	void RenderManager::drawColliders() const
	{
		if (colliders.size() == 0)
			return;

		GLDisable(GL_DEPTH_TEST);

		std::shared_ptr<Resources::ShaderProgram> program = (*colliders.begin())->getProgram();

		if (!program->bind())
			return;

		getCurrentCamera()->sendProjToProgram(program);

		for (const auto& rendererCollider : colliders)
		{
			if (rendererCollider->canBeDraw())
				rendererCollider->draw();
		}

		program->unbind();

		GLEnable(GL_DEPTH_TEST);
	}

	void RenderManager::linkComponent(Light* compToLink)
	{
		// Insert light to render
		instance()->lights.insert(compToLink);
	}

	void RenderManager::linkComponent(ModelRenderer* compToLink)
	{
		// Insert model to renderer
		instance()->models.insert(compToLink);
	}

	void RenderManager::linkComponent(SpriteRenderer* compToLink)
	{
		// Insert sprite to renderer
		instance()->sprites.insert(compToLink);
	}

	void RenderManager::linkComponent(Camera* compToLink)
	{
		// Insert camera to render
		instance()->cameras.insert(compToLink);
	}

	void RenderManager::linkComponent(SkyBox* compToLink)
	{
		// Insert camera to render
		instance()->skyBoxes.insert(compToLink);
	}

	void RenderManager::linkComponent(ColliderRenderer* compToLink)
	{
		// Insert camera to render
		instance()->colliders.insert(compToLink);
	}

	void RenderManager::removeComponent(ColliderRenderer* compToRemove)
	{
		instance()->colliders.erase(compToRemove);
	}

	void RenderManager::removeComponent(SpriteRenderer* compToRemove)
	{
		instance()->sprites.erase(compToRemove);
	}

	void RenderManager::removeComponent(ModelRenderer* compToRemove)
	{
		instance()->models.erase(compToRemove);
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

	Camera* RenderManager::getCurrentCamera()
	{
		RenderManager* RM = instance();

		if (RM->cameras.size() > 0)
			return *RM->cameras.rbegin();

		return nullptr;
	}

	void RenderManager::bindUBO(Resources::ShaderProgram* program, const std::string& UBOName)
	{
		RenderManager* RM = instance();

		if (!program->bindToUBO(UBOName, RM->lastBindingPoint))
			return;

		if (RM->uniformBlocks.find(UBOName) != RM->uniformBlocks.end())
			return;

		RM->uniformBlocks[UBOName] = UniformBlock(RM->lastBindingPoint, sizeof(LightData) * 8);

		RM->lastBindingPoint++;
	}
}