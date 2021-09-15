#pragma once

#include <deque>
#include <unordered_map>
#include <queue>

#include "game_object.hpp"

namespace Resources
{
	class Scene
	{
	private:
		std::string curGoName = "";

		void setGameObjectParent(const std::string& goName, const std::string& goChildName);

		std::queue<Engine::Object*> destroyQueue;

	public:
		bool cursorState = true;
		bool isLoadFinished = false;

		std::string filePath;
		std::unordered_map<std::string, Engine::GameObject> gameObjects;

		Scene(const std::string& path);
		~Scene();

		void load(const std::string& filePath);
		void save();
		void draw() const;
		void update();
		void fixedUpdate();

		void cleanObjects();
		Engine::GameObject* findGameObjectWithName(const std::string& gameObjectName);

		void deleteGameObject(const std::string& goName);

		void drawHierarchy();
		void drawInspector();

		void addToDestroyQueue(Engine::Object* objToDestroy);

		std::string getUniqueGOName(const std::string& gameObjectName);
		Engine::GameObject& instantiate(const std::string& gameObjectName = "GameObject");
		Engine::GameObject& instantiate(const std::string& gameObjectName, const std::string& recipePath);
	};
}