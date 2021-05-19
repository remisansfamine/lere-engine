#pragma once

#include <deque>
#include <unordered_map>

#include "game_object.hpp"

namespace Resources
{
	class Scene
	{
	private:
		int curGameObjectIndex = 0;

		void setGameObjectParent(const std::string& goName, const std::string& goChildName);

	public:
		bool cursorState = true;

		std::string filePath;
		std::deque<Engine::GameObject> gameObjects;

		Scene(const std::string& path);
		~Scene();

		void load(const std::string& filePath);
		void save();
		void draw() const;
		void update();
		void fixedUpdate();

		Engine::GameObject* findGameObjectWithName(const std::string& gameObjectName);

		void drawImGui();

		Engine::GameObject& addGameObject(const std::string& gameObjectName);
	};
}