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
#include "inputs_manager.hpp"
#include "thread_pool.hpp"
#include "debug.hpp"

#include "player_movement.hpp"
#include "main_menu.hpp"
#include "transform.hpp"
#include "sky_box.hpp"
#include "button.hpp"
#include "light.hpp"

#include "graph.hpp"

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
		gameObjects[goChildName].getComponent<Physics::Transform>()->setParent(gameObjects[goName]);
		gameObjects[goName].getComponent<Physics::Transform>()->setChild(gameObjects[goChildName]);
	}

	void Scene::clear()
	{
		gameObjects.clear();
		curGoName = "";

	}

	void Scene::load(const std::string& _filePath)
	{
		clear();

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

				Engine::GameObject& gameObject = instantiate(goName);
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

		isLoadFinished = true;
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
			scnFlux << gameObject.second.toString();
		}

		scnFlux.close();
	}

	void Scene::draw() const
	{
		if (!isLoadFinished)
			return;

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glPolygonMode(GL_FRONT, GL_FILL);  // GL_FILL | GL_LINE (fill or wireframe)
		glEnable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		LowRenderer::RenderManager::draw();
	}

	void Scene::drawHierarchy()
	{
		for (auto& gameObject : gameObjects)
			gameObject.second.drawImGuiHierarchy(curGoName, true);
	}

	void Scene::drawInspector()
	{
		if (curGoName == "")
			return;

		gameObjects[curGoName].drawImGuiInspector();
	}

	void Scene::addToDestroyQueue(Engine::Object* objToDestroy)
	{
		destroyQueue.push(objToDestroy);
	}

	void Scene::cleanObjects()
	{
		while (!destroyQueue.empty())
		{
			destroyQueue.front()->onDestroy();
			destroyQueue.pop();
		}
	}

	void Scene::deleteGameObject(const std::string& goName)
	{
		auto objIt = gameObjects.find(goName);

		if (objIt == gameObjects.end())
		{
			Core::Debug::Log::error("Game object " + goName + " not found");
			return;
		}

		gameObjects.erase(objIt);

		curGoName = "";
	}

	void Scene::update()
	{
		if (!isLoadFinished)
			return;

		for (auto& go : gameObjects)
		{
			if (go.second.isActive())
				go.second.updateComponents();
		}

		for (auto& go : gameObjects)
		{
			if (go.second.isActive())
				go.second.lateUpdateComponents();
		}
	}

	void Scene::fixedUpdate()
	{
		for (auto& go : gameObjects)
		{
			if (go.second.isActive())
				go.second.fixedUpdateComponents();
		}
	}

	std::string Scene::getUniqueGOName(const std::string& gameObjectName)
	{
		std::string modifiedName = gameObjectName;

		auto goIt = gameObjects.find(modifiedName);
		for (int count = 1; goIt != gameObjects.end(); count++, goIt = gameObjects.find(modifiedName))
			modifiedName = gameObjectName + "(" + std::to_string(count) + ")";

		return modifiedName;
	}

	Engine::GameObject& Scene::instantiate(const std::string& gameObjectName)
	{
		std::string finalName = getUniqueGOName(gameObjectName);

		gameObjects[finalName] = Engine::GameObject(finalName);
		return gameObjects[finalName];
	}

	Engine::GameObject& Scene::instantiate(const std::string& gameObjectName, const std::string& recipePath)
	{
		Engine::GameObject& go = instantiate(gameObjectName);

		std::string parentName;
		go.parseRecipe(recipePath, parentName);

		if (parentName != "" && parentName != "none")
			setGameObjectParent(parentName, go.m_name);

		return go;
	}

	Engine::GameObject* Scene::findGameObjectWithName(const std::string& gameObjectName)
	{
		/*for (auto& gameObject : gameObjects)
		{
			if (gameObjectName.compare(gameObject.second.m_name) == 0)
				return &gameObject;
		}*/

		if (gameObjects.find(gameObjectName) == gameObjects.end())
			return nullptr;

		return &gameObjects[gameObjectName];
	}
}