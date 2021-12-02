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
		entities.clear();
	}

	void Scene::setEntityParent(const std::string& goName, const std::string& goChildName)
	{
		Physics::TransformComponent* childTransform = entities[goChildName].getComponent<Physics::TransformComponent>();
		Physics::TransformComponent* parentTransform = entities[goName].getComponent<Physics::TransformComponent>();

		if (!childTransform || !parentTransform)
			return;

		childTransform->setParent(parentTransform);
		parentTransform->setChild(childTransform);
	}

	void Scene::clear()
	{
		entities.clear();
		curEntityName = "";

		std::queue<Engine::Object*> emptyQueue;
		destroyQueue.swap(emptyQueue);
	}

	void Scene::load(const std::string& _filePath)
	{
		std::ifstream scnStream(ResourcesManager::getResourcesPath() + _filePath);

		Core::Debug::Assertion::out(scnStream.is_open() && !scnStream.fail(), "Can not find scene at " + _filePath);

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

				Engine::Entity& owner = instantiate(goName);
				owner.parse(scnStream, parentName);

				if (parentName == "" || parentName == "none")
					continue;

				parents.push_back(parentName);
				parents.push_back(goName);
			}
				
		}

		for (size_t i = 0; i < parents.size(); i += 2)
			setEntityParent(parents[i], parents[i + 1]);

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

		for (auto& entity : entities)
		{
			scnFlux << entity.second.toString();
		}

		scnFlux.close();
	}

	void Scene::draw() const
	{
		if (!isLoadFinished)
			return;

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glPolygonMode(GL_FRONT, GL_FILL);
		LowRenderer::RenderManager::GLEnable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		LowRenderer::RenderManager::GLEnable(GL_BLEND);

		LowRenderer::RenderManager::GLEnable(GL_CULL_FACE);

		LowRenderer::RenderManager::draw();
	}

	void Scene::drawHierarchy()
	{
		for (auto& entity : entities)
			entity.second.drawImGuiHierarchy(curEntityName, true);
	}

	void Scene::drawInspector()
	{
		if (curEntityName == "")
			return;

		entities[curEntityName].drawImGuiInspector();
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

	void Scene::deleteEntity(const std::string& goName)
	{
		auto objIt = entities.find(goName);

		if (objIt == entities.end())
		{
			Core::Debug::Log::error("Game object " + goName + " not found");
			return;
		}

		entities.erase(objIt);

		curEntityName = "";
	}

	void Scene::update()
	{
		if (!isLoadFinished)
			return;

		for (auto& entity : entities)
		{
			if (entity.second.isActive())
				entity.second.updateComponents();
		}

		for (auto& entity : entities)
		{
			if (entity.second.isActive())
				entity.second.lateUpdateComponents();
		}
	}

	void Scene::fixedUpdate()
	{
		for (auto& entity : entities)
		{
			if (entity.second.isActive())
				entity.second.fixedUpdateComponents();
		}
	}

	std::string Scene::getUniqueEntityName(const std::string& entityName)
	{
		std::string modifiedName = entityName;

		auto goIt = entities.find(modifiedName);
		for (int count = 1; goIt != entities.end(); count++, goIt = entities.find(modifiedName))
			modifiedName = entityName + "(" + std::to_string(count) + ")";

		return modifiedName;
	}

	Engine::Entity& Scene::instantiate(const std::string& entityName)
	{
		std::string finalName = getUniqueEntityName(entityName);
		
		auto entityIt = entities.emplace(finalName, finalName);

		return (*entityIt.first).second;
	}

	Engine::Entity& Scene::instantiate(const std::string& entityName, const std::string& recipePath)
	{
		Engine::Entity& entity = instantiate(entityName);

		std::string parentName;
		entity.parseRecipe(recipePath, parentName);

		if (parentName != "" && parentName != "none")
			setEntityParent(parentName, entity.m_name);

		return entity;
	}

	Engine::Entity* Scene::findEntityWithName(const std::string& entityName)
	{
		if (entities.find(entityName) == entities.end())
			return nullptr;

		return &entities[entityName];
	}
}