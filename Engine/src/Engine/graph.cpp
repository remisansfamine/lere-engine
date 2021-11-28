#include "graph.hpp"

#include "imgui.h"

#include "resources_manager.hpp"
#include "physic_manager.hpp"
#include "render_manager.hpp"
#include "thread_manager.hpp"
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

	void Graph::reloadScene(bool wipeAll)
	{
		Graph* graph = instance();

		graph->loadScene(graph->curScene.filePath, wipeAll);
	}

	void Graph::loadScene(const std::string& scenePath, bool wipeAll)
	{
		Multithread::ThreadManager::syncAndClean("load");

		LowRenderer::RenderManager::clearAll();
		Physics::PhysicManager::clearAll();

		Resources::ResourcesManager::clearResources();

		curScene.clear();

		if (wipeAll)
			Resources::ResourcesManager::purgeResources();

		curScene.load(scenePath);

		Core::TimeManager::resetTime();
	}

	void Graph::setLoadScene(const std::string& scenePath)
	{
		instance()->sceneToLoad = scenePath;

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
		// Update the scene
		instance()->curScene.update();
	}

	void Graph::afterFrame()
	{
		Graph* graph = instance();

		if (!graph->sceneToLoad.empty())
		{
			graph->loadScene(graph->sceneToLoad);
			graph->sceneToLoad.clear();
		}
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

	void Graph::deleteEntity(const std::string& entityName)
	{
		instance()->curScene.deleteEntity(entityName);
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

			if (ImGui::Button("Reload current scene"))
				reloadScene(false);

			if (ImGui::Button("Wipe current scene"))
				reloadScene(true);

			if (ImGui::CollapsingHeader("Hierarchy"))
				graph->curScene.drawHierarchy();
		}
		ImGui::End();

		if (ImGui::Begin("Inspector"))
			graph->curScene.drawInspector();

		ImGui::End();
	}

	::Engine::Entity* Graph::findEntityWithName(const std::string& entityName)
	{
		return instance()->curScene.findEntityWithName(entityName);
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

	::Engine::Entity& Graph::instantiate(const std::string& GOname)
	{
		return instance()->curScene.instantiate(GOname);
	}

	::Engine::Entity& Graph::instantiate(const std::string& GOname, const std::string& recipePath)
	{
		return instance()->curScene.instantiate(GOname, recipePath);
	}
}