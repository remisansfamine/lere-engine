#pragma once

#include "component.hpp"

namespace Gameplay
{
	class MainMenu : public Engine::Component
	{
	private:
	public:
		MainMenu(Engine::Entity& owner);

		void start() override;
		void update() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}