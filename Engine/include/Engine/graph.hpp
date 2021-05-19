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

		Resources::Scene curScene = Resources::Scene("resources/scenes/mainMenu.scn");

	public:
		void loadScene(const std::string& scenePath);

		static void saveCurrentScene();
		static void loadNewGame();
		static void loadSaveGame();
		static void loadMainMenu();

		static void init();

		static Resources::Scene& getCurScene();

		static void draw();
		static void drawImGui();
		static void update();
		static void fixedUpdate();
		static ::Engine::GameObject* findGameObjectWithName(const std::string& gameObjectName);

		static bool getCursorState();
		static void setCursorState(bool state);
	};
}