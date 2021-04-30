#pragma once

#include "singleton.hpp"
#include "scene.hpp"

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

		static void draw();
		static void update();
	};
}