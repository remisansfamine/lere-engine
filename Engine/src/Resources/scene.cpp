#include "scene.hpp"

#include <sstream>
#include <fstream>
#include <istream>

#include "imgui.h"

#include "render_manager.hpp"
#include "model_renderer.hpp"
#include "sprite_renderer.hpp"
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
		//load("resources/scenes/test.scn");
		// Adding platforms
		{
			auto& platform0 = addGameObject("Platform0");
			platform0.addComponent<LowRenderer::ModelRenderer>("resources/obj/platforms/platform1.obj", "shader");
			platform0.addComponent<Physics::BoxCollider>();

			auto transform0 = platform0.getComponent<Physics::Transform>();
			transform0->m_scale.x = 4.f;
			transform0->m_scale.z = 3.f;

			// -----------------------------------------------------------------------------------

			auto& platform1 = addGameObject("Platform1");
			platform1.addComponent<LowRenderer::ModelRenderer>("resources/obj/platforms/platform1.obj", "shader");
			platform1.addComponent<Physics::BoxCollider>();

			auto transform1 = platform1.getComponent<Physics::Transform>();
			transform1->m_position.x = 15.f;
			transform1->m_scale.x = 10.f;
			transform1->m_scale.z = 4.f;

			// -----------------------------------------------------------------------------------

			auto& platform2 = addGameObject("Platform2");
			platform2.addComponent<LowRenderer::ModelRenderer>("resources/obj/platforms/platform0.obj", "shader");
			platform2.addComponent<Physics::BoxCollider>();

			auto transform2 = platform2.getComponent<Physics::Transform>();
			transform2->m_position.x = 13.f;
			transform2->m_position.y = 4.f;
			transform2->m_scale.x = 3.f;
			transform2->m_scale.z = 4.f;
			transform2->m_scale.y = 0.5f;

			// -----------------------------------------------------------------------------------

			auto& platform3 = addGameObject("Platform3");
			platform3.addComponent<LowRenderer::ModelRenderer>("resources/obj/platforms/platform0.obj", "shader");
			platform3.addComponent<Physics::BoxCollider>();

			auto transform3 = platform3.getComponent<Physics::Transform>();
			transform3->m_position.x = 18.f;
			transform3->m_position.y = 7.f;
			transform3->m_scale.x = 3.f;
			transform3->m_scale.z = 4.f;
			transform3->m_scale.y = 0.5f;

			// -----------------------------------------------------------------------------------

			auto& platform4 = addGameObject("Platform4");
			platform4.addComponent<LowRenderer::ModelRenderer>("resources/obj/platforms/platform2.obj", "shader");
			platform4.addComponent<Physics::BoxCollider>();

			auto transform4 = platform4.getComponent<Physics::Transform>();
			transform4->m_position.x = 6.f;
			transform4->m_position.y = 9.f;
			transform4->m_scale.x = 5.f;
			transform4->m_scale.z = 4.f;
			transform4->m_scale.y = 0.8f;
		}

		// Adding player and camera attached
		{
			auto& player = addGameObject("Player");
			player.addComponent<LowRenderer::ModelRenderer>("resources/obj/player/among_us.obj", "shader");
			player.addComponent<Gameplay::PlayerMovement>();
			player.addComponent<Physics::SphereCollider>();

			player.getComponent<Physics::Rigidbody>()->isAwake = true;

			auto transform = player.getComponent<Physics::Transform>();
			transform->m_position.y = 4.f;

			// Player creation
			auto& camera = addGameObject("MainCamera");
			camera.addComponent<LowRenderer::Camera>();

			auto camTransform = camera.getComponent<Physics::Transform>();
			camTransform->setParent(player);
			camTransform->m_position.z = 20.f;
			camTransform->m_position.y = 2.f;
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

			auto& skyBox = addGameObject("SkyBox");
			skyBox.addComponent<LowRenderer::SkyBox>(paths);
		}

		// UI creation
		{
			auto& sprite = addGameObject("Sprite");
			sprite.addComponent<LowRenderer::SpriteRenderer>("spriteShader", "resources/obj/craftsman/BLACKSMITH_TEX.jpg");
		}

		// Light creation
		{
			auto& light = addGameObject("Light");

			light.addComponent<LowRenderer::Light>();

			auto transform = light.getComponent<Physics::Transform>();
			transform->m_position.y = 1.f;

			//light.getComponent<LowRenderer::Light>()->diffuse = LowRenderer::Color(0.f, 0.f, 1.f, 1.f);
			light.getComponent<LowRenderer::Light>()->setAsDirectionnal();
		}
	}

	Scene::~Scene()
	{
		save();

		gameObjects.clear();
	}

	void Scene::setGameObjectParent(const std::string& goName, const std::string& goChildName)
	{
		size_t indexGO = 0;
		size_t indexGOChild = 0;
		int isFind = 0;

		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i].m_name == goName)
			{
				indexGO = i;
				isFind++;
			}
			if (gameObjects[i].m_name == goChildName)
			{
				indexGOChild = i;
				isFind++;
			}

			if (isFind == 2) 
				break;
		}

		gameObjects[indexGOChild].getComponent<Physics::Transform>()->setParent(gameObjects[indexGO]);
	}

	void Scene::load(const std::string& filePath)
	{
		LowRenderer::RenderManager::clearComponents<LowRenderer::SpriteRenderer>();
		LowRenderer::RenderManager::clearComponents<LowRenderer::ModelRenderer>();
		LowRenderer::RenderManager::clearComponents<LowRenderer::Camera>();
		LowRenderer::RenderManager::clearComponents<LowRenderer::Light>();
		Physics::PhysicManager::clearComponents<Physics::Rigidbody>();

		std::ifstream scnStream(filePath);
		if (!scnStream)
		{
			std::cout << "ERROR::LOAD_SCENE_FAILED : " << filePath << std::endl;
			scnStream.close();
			exit(0);
		}

		std::string line;
		std::string type;
		std::vector<std::string> parents;

		while (std::getline(scnStream, line))
		{
			if (line == "") continue;

			std::istringstream iss(line);
			iss >> type;

			if (type == "GO")
			{
				std::string goName, parentName;
				iss >> goName;

				Engine::GameObject& gameObject = addGameObject(goName);
				gameObject.parse(scnStream, parentName);

				if (parentName == "" || parentName == "none")
					continue;

				parents.push_back(parentName);
				parents.push_back(goName);
			}
				
		}

		for (size_t i = 0; i < parents.size(); i += 2)
			setGameObjectParent(parents[i], parents[i + 1]);

		scnStream.close();
	}

	void Scene::save()
	{
		std::ofstream scnFlux("resources/scenes/test.scn");

		if (!scnFlux)
		{
			std::cout << "ERROR : Can't save the scene at resources/scenes/test.scn" << std::endl;
			scnFlux.close();
			return;
		}

		for (auto& gameObject : gameObjects)
		{
			scnFlux << gameObject.toString();
		}

		scnFlux.close();
	}

	void Scene::draw() const
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
	}

	void Scene::fixedUpdate()
	{
		for (Engine::GameObject& go : gameObjects)
			go.fixedUpdateComponents();
	}

	Engine::GameObject& Scene::addGameObject(std::string gameObjectName)
	{
		return *gameObjects.insert(gameObjects.end(), Engine::GameObject(gameObjectName));
	}
}