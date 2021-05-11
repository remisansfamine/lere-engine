#include "graph.hpp"

#include "imgui.h"

#include "debug.hpp"
#include "physic_manager.hpp"

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
		// TODO
	}

	void Graph::saveCurrentScene()
	{
		// TODO
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

		// Update rigidbodies and colliders
		Physics::PhysicManager::update();
	}

	void Graph::fixedUpdate()
	{
		// Fixed update the scene
		instance()->curScene.fixedUpdate();
	}

	void Graph::drawImGui()
	{
		if (ImGui::Begin("Inspector"))
			instance()->curScene.drawImGui();

		ImGui::End();
	}

	::Engine::GameObject* Graph::findGameObjectWithName(const std::string& gameObjectName)
	{
		return instance()->curScene.findGameObjectWithName(gameObjectName);
	}
}