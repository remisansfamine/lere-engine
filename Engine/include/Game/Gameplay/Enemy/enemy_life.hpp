#pragma once

#include "component.hpp"
#include "entity_life.hpp"
#include "enemy_state.hpp"

namespace Gameplay 
{
	class EnemyLife : public EntityLife
	{
	private:
		int life = 0;

	public:
		EnemyLife(Engine::GameObject& gameObject);

		void start() override {}
		void update() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

		void onCollisionEnter(const Physics::Collision& collision) override {}
		void onCollisionExit(const Physics::Collision& collision) override {}
	};
}