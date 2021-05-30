#pragma once

#include <deque>
#include <unordered_map>

#include "game_object.hpp"

namespace Resources
{
	class Scene
	{
	private:
		std::string curGoName = "";

		void setGameObjectParent(const std::string& goName, const std::string& goChildName);

		std::vector<Engine::Object*> objectsToDestroy;

	public:
		bool cursorState = true;

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

		Engine::GameObject& addGameObject(const std::string& gameObjectName);
	};
}