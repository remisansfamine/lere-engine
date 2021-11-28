#pragma once

#include "player_state.hpp" 
#include "entity_movement.hpp"

namespace Gameplay
{
	class PlayerMovement : public EntityMovement
	{
	private:
		float m_jumpForce = 500.f;
		float m_sensivityY = 0.1f;

		Gameplay::PlayerState* m_playerState;
		Physics::TransformComponent* m_cameraTransform;

	public:
		PlayerMovement(Engine::Entity& owner);

		void start() override;
		void fixedUpdate() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);

	};
}