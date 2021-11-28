#pragma once

#include "component.hpp"
#include "game_master.hpp"

namespace Gameplay
{
	class LoseScreen : public Engine::Component
	{	
	public:
		Engine::Entity* buttons[3] = { nullptr };
	
		LoseScreen(Engine::Entity& owner);
	
		void start() override;
		void drawImGui() override;
	
		void showLoseScreen(bool isActive);
	
		std::string toString() const override;
	
		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}