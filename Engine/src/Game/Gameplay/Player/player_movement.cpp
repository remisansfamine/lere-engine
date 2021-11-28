#include "player_movement.hpp"

namespace Gameplay
{
	PlayerMovement::PlayerMovement(Engine::Entity& owner)
		: EntityMovement(owner)
	{
		m_playerState = requireComponent<Gameplay::PlayerState>();
		m_transform = m_playerState->transform;
	}

	void PlayerMovement::start()
	{
		m_cameraTransform = Core::Engine::Graph::findEntityWithName("MainCamera")->getComponent<Physics::TransformComponent>();
	}

	void PlayerMovement::fixedUpdate()
	{
		float fixedSpeed = m_speed * Core::TimeManager::getFixedDeltaTime();
		float horizontal = m_playerState->horizontalMove;
		float vertical = m_playerState->forwardMove;

		Core::Maths::vec3 rotation = m_transform->rotation;
		rotation.y -= m_sensivityY * Core::TimeManager::getFixedDeltaTime() * Core::Input::InputManager::getDeltasMouse().x;
		m_transform->rotation = rotation;

		float cos = cosf(rotation.y), sin = sinf(rotation.y);
		Core::Maths::vec3 newVelocity = Core::Maths::vec3(horizontal * cos + vertical * sin, 0.f, vertical * cos - horizontal * sin).normalized() * fixedSpeed;
		newVelocity.y = m_rigidbody->velocity.y;
		m_rigidbody->velocity = newVelocity;

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
			ImGui::DragFloat("Sensivity Y : ", &m_sensivityY);
			ImGui::DragFloat("MoveSpeed : ", &m_speed);
			ImGui::DragFloat("JumpForce : ", &m_jumpForce);
			ImGui::TreePop();
		}
	}

	std::string PlayerMovement::toString() const
	{
		return "COMP PLAYERMOVEMENT " + std::to_string(m_speed) + " " + std::to_string(m_jumpForce) + " " + std::to_string(m_sensivityY);
	}

	void PlayerMovement::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		PlayerMovement* player;
		if (!owner.tryGetComponent(player))
			player = owner.addComponent<PlayerMovement>();

		iss >> player->m_speed;
		iss >> player->m_jumpForce;
		iss >> player->m_sensivityY;
	}
}