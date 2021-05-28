#pragma once

#include "enemy_state.hpp"
#include "entity_movement.hpp"

namespace Gameplay
{
	class EnemyMovement : public EntityMovement
	{
	private:
		std::shared_ptr<Gameplay::EnemyState> m_enemyState;

	public:
		EnemyMovement(Engine::GameObject& gameObject);

		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

		//void onTriggerEnter(const Physics::Collision& collision) override;
		//void onTriggerExit(const Physics::Collision& collision) override;		
		void onCollisionEnter(const Physics::Collision& collision) override;
		void onCollisionExit(const Physics::Collision& collision) override;
	};
}