#pragma once

#include "player_state.hpp" 
#include "entity_movement.hpp"

namespace Gameplay
{
	class PlayerMovement : public EntityMovement
	{
	private:
		float m_jumpForce = 500.f;

		std::shared_ptr<Gameplay::PlayerState> m_playerState;
		std::shared_ptr<Physics::Transform> m_cameraTransform;

	public:
		PlayerMovement(Engine::GameObject& gameObject);

		void start() override;
		void fixedUpdate() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

	};
}