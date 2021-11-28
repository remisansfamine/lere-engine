#pragma once

#include "component.hpp"
#include "entity_life.hpp"
#include "enemy_state.hpp"

#include "game_master.hpp"

namespace Gameplay 
{
	class EnemyLife : public EntityLife
	{
	private:
		void kill() override;
		GameMaster* gameMaster = nullptr;

	public:
		EnemyLife(Engine::Entity& owner);

		void start() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);

		void onCollisionEnter(const Physics::Collision& collision) override {}
		void onCollisionExit(const Physics::Collision& collision) override {}

		void hurt(int damage = 1) override;
	};
}