#pragma once

#include "component.hpp"

#include "timer.hpp"

namespace Gameplay
{
	class BulletHole : public Engine::Component

	{
		Core::Timer lifeTime;

	public:
		BulletHole(Engine::Entity& owner);

		void start() override;
		void update() override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}