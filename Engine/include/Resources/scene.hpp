#pragma once

#include <deque>
#include <unordered_map>
#include <queue>

#include "entity.hpp"

namespace Resources
{
	class Scene
	{
	private:
		std::string curEntityName = "";

		void setEntityParent(const std::string& entityName, const std::string& entityChildName);

		std::queue<Engine::Object*> destroyQueue;

	public:
		bool stopAllUpdate = false;

		bool cursorState = true;
		bool isLoadFinished = false;

		std::string filePath;
		std::unordered_map<std::string, Engine::Entity> entities;

		Scene(const std::string& path);
		~Scene();

		void clear();
		void load(const std::string& filePath);
		void save();
		void draw() const;
		void update();
		void fixedUpdate();

		void cleanObjects();
		Engine::Entity* findEntityWithName(const std::string& entityName);

		void deleteEntity(const std::string& goName);

		void drawHierarchy();
		void drawInspector();

		void addToDestroyQueue(Engine::Object* objToDestroy);

		std::string getUniqueEntityName(const std::string& entityName);
		Engine::Entity& instantiate(const std::string& entityName = "Entity");
		Engine::Entity& instantiate(const std::string& entityName, const std::string& recipePath);
	};
}