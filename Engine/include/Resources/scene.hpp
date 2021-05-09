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
		std::string name;
		std::deque<Engine::GameObject> gameObjects;

		Scene(const std::string& path);
		Scene();
		~Scene();

		// TODO
		void load(const std::string& filePath);
		void save();
		void draw() const;
		void update();
		void fixedUpdate();

		void drawImGui();

		Engine::GameObject& addGameObject(std::string gameObjectName);
	};
}