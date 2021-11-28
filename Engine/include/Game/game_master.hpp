#pragma once

#include "component.hpp"

namespace Gameplay
{
	class PauseScreen;
	class WinScreen;
	class LoseScreen;

	class GameMaster : public Engine::Component
	{
	private:
		PauseScreen* pauseScreen = nullptr;
		WinScreen* winScreen = nullptr;
		LoseScreen* loseScreen = nullptr;

	public:
		GameMaster(Engine::Entity& owner);

		bool isPaused = false;
		int enemyCount = 0;

		void start() override;
		void update() override;
		void drawImGui() override;
		void removeEnemy();
		void removePlayer();

		std::string toString() const override;
		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}