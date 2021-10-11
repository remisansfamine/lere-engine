#include "graph_wrapper.hpp"

#include "graph.hpp"
#include "debug.hpp"

namespace Scripting
{
	namespace Graph
	{
		void loadScene(const std::string& sceneToLoad)
		{
			Core::Engine::Graph::loadScene(sceneToLoad);
		}
	}
}