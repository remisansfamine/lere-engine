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
		
		bool isStartingNewGame = false;
		bool isLoadingSavedScene = false;
		bool isLoadingMainMenu = false;

		bool showDemoWindowImGui = false;

		Resources::Scene curScene = Resources::Scene("resources/scenes/mainMenu.scn");

	public:
		static void reloadScene();

		void loadScene(const std::string& scenePath);

		static void saveCurrentScene();
		static void loadNewGame();
		static void loadSaveGame();
		static void loadMainMenu();

		static void init();

		static Resources::Scene& getCurScene();

		static void draw();
		static void update();
		static void drawImGui();
		static void fixedUpdate();
		static void clean();
		static void deleteGameObject(const std::string& goName);

		static ::Engine::GameObject* findGameObjectWithName(const std::string& gameObjectName);

		static void addToDestroyQueue(::Engine::Object* obj);

		static bool getCursorState();
		static void setCursorState(bool state);

		static ::Engine::GameObject& instantiate(const std::string& GOname);
		static ::Engine::GameObject& instantiate(const std::string& GOname, const std::string& recipePath);
	};
}