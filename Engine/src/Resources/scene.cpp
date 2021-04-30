#include "scene.hpp"

#include "debug.hpp"
#include "render_manager.hpp"

#include "sprite_renderer.hpp"
#include "model_renderer.hpp"

namespace Resources
{
	Scene::Scene(const std::string& path)
	{
		Core::Debug::Log::info("Loading " + path);
	}

	Scene::Scene()
	{
		gameObjects.push_back(Engine::GameObject("LERE"));
	}

	Scene::~Scene()
	{

	}

	void Scene::load(const std::string& _name)
	{
		// TODO: load scene from name

		LowRenderer::RenderManager::clearComponents<LowRenderer::Renderer>();
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