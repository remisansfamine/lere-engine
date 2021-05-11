#include "player_movement.hpp"

#include "imgui.h"

#include "inputs_manager.hpp"
#include "time.hpp"

namespace Gameplay
{
	PlayerMovement::PlayerMovement(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<PlayerMovement>(this))
	{
		m_rigidbody = requireComponent<Physics::Rigidbody>();
		m_playerState = requireComponent<Gameplay::PlayerState>();
	}

	void PlayerMovement::fixedUpdate()
	{
		float fixedDeltaTime = Core::TimeManager::getFixedDeltaTime();

		float fixedSpeed = m_speed * fixedDeltaTime;

		m_rigidbody->velocity = Core::Maths::vec3(m_playerState->horizontalMove * fixedSpeed, m_rigidbody->velocity.y, m_playerState->forwardMove * fixedSpeed);

		if (m_playerState->isGrounded && m_playerState->isJumping)
		{
			m_rigidbody->addForce(Core::Maths::vec3(0.f, m_jumpForce, 0.f));
			m_playerState->isJumping = false;
		}
	}

	void PlayerMovement::drawImGui()
	{
		if (ImGui::TreeNode("PlayerMovement"))
		{
			ImGui::DragFloat("MoveSpeed : ", &m_speed);
			ImGui::DragFloat("JumpForce : ", &m_jumpForce);
			ImGui::TreePop();
		}
	}

	std::string PlayerMovement::toString() const
	{
		return "COMP PLAYERMOVEMENT " + std::to_string(m_speed) + " " + std::to_string(m_jumpForce);
	}

	void PlayerMovement::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		gameObject.addComponent<PlayerMovement>();
		auto player = gameObject.getComponent<PlayerMovement>();

		iss >> player->m_speed;
		iss >> player->m_jumpForce;
	}
}