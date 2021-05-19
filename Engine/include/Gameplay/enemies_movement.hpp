#pragma once

#include "component.hpp"
#include "rigidbody.hpp"
#include "enemies_state.hpp"

namespace Gameplay
{
	class EnemiesMovement : public Engine::Component
	{

	private:
		float m_speed = 350.f;
		float m_jumpForce = 500.f;

		std::shared_ptr<Physics::Rigidbody> m_rigidbody;
		std::shared_ptr<Gameplay::EnemiesState> m_enemiesState;
		std::shared_ptr<Physics::Transform> m_cameraTransform;

	public:
		EnemiesMovement(Engine::GameObject& gameObject);

		void start() override;
		void fixedUpdate() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}