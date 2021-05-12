#include "player_movement.hpp"

#include "imgui.h"

#include "inputs_manager.hpp"
#include "graph.hpp"
#include "time.hpp"

namespace Gameplay
{
	PlayerMovement::PlayerMovement(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<PlayerMovement>(this))
	{
		m_rigidbody = requireComponent<Physics::Rigidbody>();
		m_playerState = requireComponent<Gameplay::PlayerState>();
	}

	void PlayerMovement::start()
	{
		m_cameraTransform = Core::Engine::Graph::findGameObjectWithName("MainCamera")->getComponent<Physics::Transform>();
	}

	void PlayerMovement::fixedUpdate()
	{
		float fixedSpeed = m_speed * Core::TimeManager::getFixedDeltaTime();
		float horizontal = m_playerState->horizontalMove * fixedSpeed;
		float vertical = -m_playerState->forwardMove * fixedSpeed;

		m_rigidbody->velocity.x = horizontal * cos(m_cameraTransform->m_rotation.y) + vertical * sin(m_cameraTransform->m_rotation.y);
		m_rigidbody->velocity.z = -vertical * cos(m_cameraTransform->m_rotation.y) + horizontal * sin(m_cameraTransform->m_rotation.y);

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