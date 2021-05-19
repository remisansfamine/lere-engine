#pragma once

#include "component.hpp"

namespace Gameplay
{
	class GameMaster : public Engine::Component
	{
	private:
	public:
		GameMaster(Engine::GameObject& gameObject);

		void start() override;
		void update() override;
		void drawImGui() override;

		std::string toString() const override;
		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}