#include "engine_master.hpp"

#include "physic_manager.hpp"
#include "render_manager.hpp"
#include "debug.hpp"
#include "graph.hpp"
#include "time.hpp"

namespace Core::Engine
{
	EngineMaster::EngineMaster()
	{
		Core::Debug::Log::info("Creating the Engine");

		TimeManager::setTimeScale(5.f);

		Physics::PhysicManager::clearColliders();
	}

	EngineMaster::~EngineMaster()
	{
		Core::Debug::Log::info("Destroying the Engine");


		// Kill managers
		Graph::kill();
		Physics::PhysicManager::kill();
		LowRenderer::RenderManager::kill();
	}

	void EngineMaster::update()
	{
		EngineMaster* EM = instance();

		Graph::update();
		Graph::draw();
	}
}	