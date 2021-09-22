#include "graph.hpp"

#include "imgui.h"

#include "physic_manager.hpp"
#include "application.hpp"
#include "debug.hpp"
#include "time.hpp"

namespace Core::Engine
{
	Graph::Graph()
	{
		Core::Debug::Log::info("Creating the Graph");
	}

	Graph::~Graph()
	{
		Core::Debug::Log::info("Destroying the Graph");
	}

	void Graph::loadScene(const std::string& scenePath)
	{
		curScene.load(scenePath);
		Core::TimeManager::resetTime();
	}

	void Graph::loadSaveGame()
	{
		instance()->isLoadingSavedScene = true;
	}

	void Graph::loadNewGame()
	{
		instance()->isStartingNewGame = true;
	}

	void Graph::loadMainMenu()
	{
		instance()->isLoadingMainMenu = true;
	}

	void Graph::saveCurrentScene()
	{
		instance()->curScene.save();
	}

	void Graph::init()
	{
		Graph* graph = instance();
	}

	Resources::Scene& Graph::getCurScene()
	{
		return instance()->curScene;
	}

	void Graph::draw()
	{
		// Draw the scene
		instance()->curScene.draw();
	}

	void Graph::update()
	{
		Graph* graph = instance();

		if (graph->isStartingNewGame)
		{
			graph->loadScene("resources/scenes/defaultScene.scn");
			saveCurrentScene();
			graph->isStartingNewGame = false;
		}
		else if (graph->isLoadingSavedScene)
		{
			graph->loadScene("resources/scenes/savedScene.scn");
			graph->isLoadingSavedScene = false;
		}
		else if (graph->isLoadingMainMenu)
		{
			graph->loadScene("resources/scenes/mainMenu.scn");
			graph->isLoadingMainMenu = false;
		}

		// Update the scene
		graph->curScene.update();

		// Update rigidbodies and colliders
		Physics::PhysicManager::update();
	}

	void Graph::fixedUpdate()
	{
		// Fixed update the scene
		instance()->curScene.fixedUpdate();
	}

	void Graph::clean()
	{
		instance()->curScene.cleanObjects();
	}

	void Graph::deleteGameObject(const std::string& goName)
	{
		instance()->curScene.deleteGameObject(goName);
	}

	void Graph::drawImGui()
	{
		Graph* graph = instance();

		if (ImGui::Begin("Graph"))
		{
			if (ImGui::Button("Show demo window"))
				graph->showDemoWindowImGui = !graph->showDemoWindowImGui;

			if (graph->showDemoWindowImGui)
				ImGui::ShowDemoWindow();

			if (ImGui::CollapsingHeader("Hierarchy"))
				graph->curScene.drawHierarchy();
		}
		ImGui::End();

		if (ImGui::Begin("Inspector"))
			graph->curScene.drawInspector();

		ImGui::End();
	}

	::Engine::GameObject* Graph::findGameObjectWithName(const std::string& gameObjectName)
	{
		return instance()->curScene.findGameObjectWithName(gameObjectName);
	}

	void Graph::addToDestroyQueue(::Engine::Object* obj)
	{
		if (!obj)
			return;

		instance()->curScene.addToDestroyQueue(obj);
	}

	bool Graph::getCursorState()
	{
		return instance()->curScene.cursorState;
	}

	void Graph::setCursorState(bool state)
	{
		instance()->curScene.cursorState = state;

		Core::Application::setCursor(state);
	}

	::Engine::GameObject& Graph::instantiate(const std::string& GOname)
	{
		return instance()->curScene.instantiate(GOname);
	}

	::Engine::GameObject& Graph::instantiate(const std::string& GOname, const std::string& recipePath)
	{
		return instance()->curScene.instantiate(GOname, recipePath);
	}
}