#pragma once

#include "component.hpp"
#include "rigidbody.hpp"
#include "enemy_state.hpp"

namespace Gameplay
{
	class EnemyMovement : public Engine::Component
	{
	private:
		float m_speed = 2.f;

		std::shared_ptr<Physics::Rigidbody> m_rigidbody;
		std::shared_ptr<Gameplay::EnemyState> m_enemyState;

	public:
		EnemyMovement(Engine::GameObject& gameObject);

		void start() override;
		void fixedUpdate() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}