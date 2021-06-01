#pragma once

#include "component.hpp"

namespace Gameplay
{
	class PauseMenu : public Engine::Component
	{
	public:
		PauseMenu(Engine::GameObject& gameObject);

		void start() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}