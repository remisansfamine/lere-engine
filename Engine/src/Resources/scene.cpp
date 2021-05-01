#include "scene.hpp"

#include "debug.hpp"
#include "render_manager.hpp"

#include "model_renderer.hpp"

namespace Resources
{
	Scene::Scene(const std::string& path)
	{
		Core::Debug::Log::info("Loading " + path);
	}

	Scene::Scene()
	{
		// Craftsman creation
		{
			Engine::GameObject craftsman("Craftsman");
			craftsman.addComponent<LowRenderer::ModelRenderer>("resources/obj/craftsman/craftsman.obj");

			gameObjects.push_back(craftsman);
		}

		// Player creation
		{
			Engine::GameObject player("Player");
			player.addComponent<LowRenderer::Camera>();

			gameObjects.push_back(player);
		}
	}

	Scene::~Scene()
	{

	}

	void Scene::load(const std::string& _name)
	{
		// TODO: load scene from name

		LowRenderer::RenderManager::clearComponents<LowRenderer::Renderer>();
		LowRenderer::RenderManager::clearComponents<LowRenderer::Camera>();
		LowRenderer::RenderManager::clearComponents<LowRenderer::Light>();
	}

	void Scene::save()
	{

	}

	void Scene::draw()
	{
		LowRenderer::RenderManager::draw();
	}

	void Scene::update()
	{
		for (Engine::GameObject& go : gameObjects)
			go.updateComponents();
	}
}