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

	}

	void Graph::saveCurrentScene()
	{
		
	}

	void Graph::init()
	{
		Graph* graph = instance();
	}

	void Graph::draw()
	{
		instance()->curScene.draw();
	}

	void Graph::update()
	{
		instance()->curScene.update();
	}
}