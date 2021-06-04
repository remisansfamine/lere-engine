#pragma once

#include "component.hpp"

namespace Gameplay
{
	class PauseScreen;

	class GameMaster : public Engine::Component
	{
	private:
		std::shared_ptr<PauseScreen> pauseScreen;

	public:
		GameMaster(Engine::GameObject& gameObject);

		bool isPaused = false;

		void start() override;
		void update() override;
		void drawImGui() override;

		std::string toString() const override;
		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}