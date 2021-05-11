#pragma once

#include "singleton.hpp"

#include "scene.hpp"
#include "game_object.hpp"

namespace Core::Engine
{
	class Graph final : public Singleton<Graph>
	{
		friend class Singleton<Graph>;

	private:
		Graph();
		~Graph();

		Resources::Scene curScene;

	public:
		void loadScene(const std::string& scenePath);
		void saveCurrentScene();
		static void init();

		static Resources::Scene& getCurScene();

		static void draw();
		static void drawImGui();
		static void update();
		static void fixedUpdate();
		static ::Engine::GameObject* findGameObjectWithName(const std::string& gameObjectName);
	};
}