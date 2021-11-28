#pragma once

#include "enemy_state.hpp"
#include "entity_movement.hpp"

namespace Gameplay
{
	class EnemyMovement : public EntityMovement
	{
	private:
		Gameplay::EnemyState* m_enemyState = nullptr;
		Physics::TransformComponent* m_target = nullptr;

	public:
		EnemyMovement(Engine::Entity& owner);

		void fixedUpdate() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);

		void onTriggerEnter(Physics::Collider* collider) override;
		void onTriggerExit(Physics::Collider* collider) override;
	};
}