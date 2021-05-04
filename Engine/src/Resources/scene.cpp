#include "scene.hpp"

#include "render_manager.hpp"
#include "model_renderer.hpp"
#include "physic_manager.hpp"
#include "debug.hpp"

#include "transform.hpp"
#include "light.hpp"

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
			craftsman.addComponent<LowRenderer::ModelRenderer>("resources/obj/craftsman/craftsman.obj", "shader");
			craftsman.addComponent<Physics::BoxCollider>();

			auto transform = craftsman.getComponent<Physics::Transform>();
			transform->m_position.z = 0.f;
			transform->m_position.x = 1.f;

			gameObjects.push_back(craftsman);
		}

		// Craftsman creation
		{
			Engine::GameObject craftsman("Craftsman2");
			craftsman.addComponent<LowRenderer::ModelRenderer>("resources/obj/craftsman/craftsman.obj", "shader");
			craftsman.addComponent<Physics::BoxCollider>();
			craftsman.addComponent<Physics::Rigidbody>();

			craftsman.getComponent<Physics::Rigidbody>()->isAwake = true;

			auto transform = craftsman.getComponent<Physics::Transform>();
			transform->m_position.z = 0.f;
			transform->m_position.x = -2.f;

			gameObjects.push_back(craftsman);
		}

		// Light creation
		{
			Engine::GameObject light("Light");
			light.addComponent<LowRenderer::Light>();

			auto transform = light.getComponent<Physics::Transform>();
			transform->m_position.z = 0.f;
			transform->m_position.x = -1.f;
			transform->m_position.y = 2.f;

			//light.getComponent<LowRenderer::Light>()->diffuse = LowRenderer::Color(0.f, 0.f, 1.f, 1.f);
			light.getComponent<LowRenderer::Light>()->setAsPoint();

			gameObjects.push_back(light);
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
		Physics::PhysicManager::clearComponents<Physics::Rigidbody>();
	}

	void Scene::save()
	{

	}

	void Scene::draw()
	{
		glClearColor(1.f, 0.3f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glPolygonMode(GL_FRONT, GL_FILL);  // GL_FILL | GL_LINE (fill or wireframe)
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_FRAMEBUFFER_SRGB);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		LowRenderer::RenderManager::draw();
	}

	void Scene::update()
	{
		for (Engine::GameObject& go : gameObjects)
			go.updateComponents();

		for (Engine::GameObject& go : gameObjects)
			go.lateUpdateComponents();
	}

	void Scene::fixedUpdate()
	{
		for (Engine::GameObject& go : gameObjects)
			go.fixedUpdateComponents();
	}
}