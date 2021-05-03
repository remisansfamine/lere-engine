#include "graph.hpp"

#include "debug.hpp"

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
}