#pragma once

#include "game_object.hpp"

namespace Resources
{
	class Scene
	{
	private:
		int curGameObjectIndex = 0;

	public:
		std::string name;
		std::vector<Engine::GameObject> gameObjects;

		Scene(const std::string& path);
		Scene();
		~Scene();

		// TODO
		void load(const std::string& _name);
		void save();
		void draw();
		void update();
		void fixedUpdate();

		void drawImGui();
	};
}