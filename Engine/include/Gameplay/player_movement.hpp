#pragma once

#include "component.hpp"
#include "rigidbody.hpp"
#include "player_state.hpp"

namespace Gameplay
{
	class PlayerMovement : public Engine::Component
	{

	private:
		float m_speed = 350.f;
		float m_jumpForce = 500.f;

		std::shared_ptr<Physics::Rigidbody> m_rigidbody;
		std::shared_ptr<Gameplay::PlayerState> m_playerState;

	public:
		PlayerMovement(Engine::GameObject& gameObject);

		void fixedUpdate() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

	};
}