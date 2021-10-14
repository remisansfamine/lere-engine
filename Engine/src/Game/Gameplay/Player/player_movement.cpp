#include "player_movement.hpp"

#include "pyhelper.hpp"

#include "resources_manager.hpp"

namespace Gameplay
{
	PlayerMovement::PlayerMovement(Engine::GameObject& gameObject)
		: EntityMovement(gameObject, std::shared_ptr<PlayerMovement>(this))
	{
		m_playerState = requireComponent<Gameplay::PlayerState>();
		m_transform = m_playerState->transform;
	}

	void PlayerMovement::start()
	{
		m_cameraTransform = Core::Engine::Graph::findGameObjectWithName("MainCamera")->getComponent<Physics::Transform>();

		script = Resources::ResourcesManager::loadScript("player_stats");
	}

	void PlayerMovement::fixedUpdate()
	{
		m_speed = script->callFunction("getSpeed").asFloat();
		m_sensivityY = script->callFunction("getSensitivity").asFloat();
		m_jumpForce = script->callFunction("getJump").asFloat();

		float fixedSpeed = m_speed * Core::TimeManager::getFixedDeltaTime();
		float horizontal = m_playerState->horizontalMove;
		float vertical = m_playerState->forwardMove;

		m_transform->m_rotation.y -= m_sensivityY * Core::TimeManager::getFixedDeltaTime() * Core::Input::InputManager::getDeltasMouse().x;

		float cos = cosf(m_transform->m_rotation.y), sin = sinf(m_transform->m_rotation.y);
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

	void PlayerMovement::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::shared_ptr<PlayerMovement> player;
		if (!gameObject.tryGetComponent(player))
			player = gameObject.addComponent<PlayerMovement>();

		iss >> player->m_speed;
		iss >> player->m_jumpForce;
		iss >> player->m_sensivityY;
	}
}