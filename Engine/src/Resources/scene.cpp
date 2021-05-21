#include "scene.hpp"

#include <sstream>
#include <fstream>
#include <istream>

#include "imgui.h"

#include "render_manager.hpp"
#include "model_renderer.hpp"
#include "sprite_renderer.hpp"
#include "resources_manager.hpp"
#include "physic_manager.hpp"
#include "Input/inputs_manager.hpp"
#include "debug.hpp"

#include "player_movement.hpp"
#include "main_menu.hpp"
#include "transform.hpp"
#include "sky_box.hpp"
#include "button.hpp"
#include "light.hpp"

namespace Resources
{
	Scene::Scene(const std::string& path)
	{
		Core::Debug::Log::info("Loading " + path);

		load(path);
	}

	Scene::~Scene()
	{
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

	void Scene::load(const std::string& _filePath)
	{
		LowRenderer::RenderManager::clearComponents<LowRenderer::SpriteRenderer>();
		LowRenderer::RenderManager::clearComponents<LowRenderer::ModelRenderer>();
		LowRenderer::RenderManager::clearComponents<LowRenderer::Camera>();
		LowRenderer::RenderManager::clearComponents<LowRenderer::Light>();
		Physics::PhysicManager::clearComponents<Physics::Rigidbody>();
		Physics::PhysicManager::clearComponents<Physics::SphereCollider>();
		Physics::PhysicManager::clearComponents<Physics::BoxCollider>();

		gameObjects.clear();
		curGameObjectIndex = 0;

		std::ifstream scnStream(_filePath);

		Core::Debug::Assertion::out(scnStream.is_open(), "Can not find scene at " + _filePath);

		filePath = _filePath;

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

		//Resources::ResourcesManager::clearResources();
	}

	void Scene::save()
	{
		std::ofstream scnFlux("resources/scenes/savedScene.scn");

		if (!scnFlux)
		{
			std::cout << "ERROR : Can't save the scene at resources/scenes/savedScene.scn" << std::endl;
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

	Engine::GameObject& Scene::addGameObject(const std::string& gameObjectName)
	{
		return *gameObjects.insert(gameObjects.end(), Engine::GameObject(gameObjectName));
	}

	Engine::GameObject* Scene::findGameObjectWithName(const std::string& gameObjectName)
	{
		for (auto& gameObject : gameObjects)
		{
			if (gameObjectName.compare(gameObject.m_name) == 0)
				return &gameObject;
		}

		return nullptr;
	}
}