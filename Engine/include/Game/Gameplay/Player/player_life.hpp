#pragma once
#include <string>

#include "entity_life.hpp"
#include "game_master.hpp"

namespace Gameplay
{
	class PlayerLife : public EntityLife
	{
	private:
		void kill() override;
		std::string lifeBarName;
		GameMaster* gameMaster = nullptr;

	public:
		PlayerLife(Engine::Entity& owner);

		void start() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}