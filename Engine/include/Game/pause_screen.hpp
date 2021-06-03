#pragma once

#include "component.hpp"

namespace Gameplay
{
	class PauseScreen : public Engine::Component
	{
	public:
		PauseScreen(Engine::GameObject& gameObject);

		void start() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}