#pragma once

#include "singleton.hpp"

#include "scene.hpp"
#include "entity.hpp"

namespace Core::Engine
{
	class Graph final : public Singleton<Graph>
	{
		friend class Singleton<Graph>;

	private:
		Graph();
		~Graph();

		bool showDemoWindowImGui = false;

		Resources::Scene curScene = Resources::Scene("resources/scenes/mainMenu.scn");

		std::string sceneToLoad;

	public:
		static void reloadScene(bool wipeAll);

		void loadScene(const std::string& scenePath, bool wipeAll = false);

		static void setLoadScene(const std::string& scenePath);

		static void init();

		static Resources::Scene& getCurScene();

		static void draw();
		static void update();
		static void afterFrame();
		static void drawImGui();
		static void fixedUpdate();
		static void clean();
		static void deleteEntity(const std::string& entityName);

		static ::Engine::Entity* findEntityWithName(const std::string& entityName);

		static void addToDestroyQueue(::Engine::Object* obj);

		static bool getCursorState();
		static void setCursorState(bool state);

		static ::Engine::Entity& instantiate(const std::string& entityName);
		static ::Engine::Entity& instantiate(const std::string& entityName, const std::string& recipePath);
	};
}