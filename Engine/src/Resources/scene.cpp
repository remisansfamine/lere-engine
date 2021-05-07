#include "scene.hpp"

#include "imgui.h"

#include "render_manager.hpp"
#include "model_renderer.hpp"
#include "physic_manager.hpp"
#include "inputs_manager.hpp"
#include "debug.hpp"

#include "player_movement.hpp"
#include "transform.hpp"
#include "sky_box.hpp"
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
			Engine::GameObject box("box");
			box.addComponent<LowRenderer::ModelRenderer>("resources/obj/cube.obj", "shader");
			box.addComponent<Physics::BoxCollider>();

			auto transform = box.getComponent<Physics::Transform>();
			transform->m_position.z = -3.f;
			transform->m_position.y = -2.f;

			gameObjects.push_back(box);
		}

		{
			Engine::GameObject sphere("sphere");
			sphere.addComponent<LowRenderer::ModelRenderer>("resources/obj/sphere.obj", "shader");
			sphere.addComponent<Gameplay::PlayerMovement>();
			sphere.addComponent<Physics::SphereCollider>();

			sphere.getComponent<Physics::Rigidbody>()->isAwake = true;

			auto transform = sphere.getComponent<Physics::Transform>();
			transform->m_position.z = -3.f;
			transform->m_position.y = 10.f;



			Engine::GameObject sphere2("sphere2");
			sphere2.addComponent<LowRenderer::ModelRenderer>("resources/obj/sphere.obj", "shader");

			auto transform2 = sphere2.getComponent<Physics::Transform>();
			transform2->m_position.x = 2.f;
			transform2->setParent(transform);



			// Player creation
			Engine::GameObject player("Player");
			player.addComponent<LowRenderer::Camera>();

			auto transformPlayer = player.getComponent<Physics::Transform>();
			//transformPlayer->setParent(transform);
			//transformPlayer->m_position.z = 5.f;

			gameObjects.push_back(player);
			gameObjects.push_back(sphere);
			gameObjects.push_back(sphere2);
		}

		/*// Craftsman creation
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
		}*/

		{
			
		}

		// SkyBox creation
		{
			std::string dir = "resources/skyboxes/";
			std::vector<std::string> paths = {
				dir + "right.jpg",
				dir + "left.jpg",
				dir + "top.jpg",
				dir + "bottom.jpg",
				dir + "front.jpg",
				dir + "back.jpg"
			};

			Engine::GameObject skyBox("SkyBox");
			skyBox.addComponent<LowRenderer::SkyBox>(paths, "skyBox");

			gameObjects.push_back(skyBox);
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
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glPolygonMode(GL_FRONT, GL_FILL);  // GL_FILL | GL_LINE (fill or wireframe)
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_FRAMEBUFFER_SRGB);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		LowRenderer::RenderManager::draw();
	}

	void Scene::drawImGui()
	{
		int goSize = (int)(gameObjects.size());

		if (goSize == 0)
			return;

		ImGui::SliderInt("Index current GameObject", &curGameObjectIndex, 0, goSize - 1);

		gameObjects[curGameObjectIndex].drawImGui();
	}

	void Scene::update()
	{
		for (Engine::GameObject& go : gameObjects)
			go.updateComponents();

		for (Engine::GameObject& go : gameObjects)
			go.lateUpdateComponents();

		gameObjects[2].getComponent<Physics::Transform>()->m_rotation.y += Core::Input::InputManager::getAxis("MoveObjectHorizontal") * 2.f * Core::Maths::DEG2RAD;
		gameObjects[2].getComponent<Physics::Transform>()->m_rotation.z += Core::Input::InputManager::getAxis("MoveObjectForward") * 2.f * Core::Maths::DEG2RAD;
		//gameObjects[0].getComponent<Physics::Transform>()->m_position.z += Core::Input::InputManager::getAxis("MoveObjectForward") * 0.1f;
		//gameObjects[0].getComponent<Physics::Transform>()->m_position.y += Core::Input::InputManager::getAxis("MoveObjectVertical") * 0.1f;
	}

	void Scene::fixedUpdate()
	{
		for (Engine::GameObject& go : gameObjects)
			go.fixedUpdateComponents();
	}
}