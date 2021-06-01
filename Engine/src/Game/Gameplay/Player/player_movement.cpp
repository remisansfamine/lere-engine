#include "player_movement.hpp"

#include "graph.hpp"

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
	}

	void PlayerMovement::fixedUpdate()
	{
		if (Core::Input::InputManager::getMouseButtonDown("RightClick"))
		Core::Engine::Graph::instantiate("agrou", "resources/recipes/enemy.recipe");



		float fixedSpeed = m_speed * Core::TimeManager::getFixedDeltaTime();
		float horizontal = m_playerState->horizontalMove * fixedSpeed;
		float vertical = m_playerState->forwardMove * fixedSpeed;

		m_transform->m_rotation.y -= 0.5f * Core::TimeManager::getFixedDeltaTime() * Core::Input::InputManager::getDeltasMouse().x;

		float cos = cosf(m_transform->m_rotation.y), sin = sinf(m_transform->m_rotation.y);
		m_rigidbody->velocity.x = horizontal * cos + vertical * sin;
		m_rigidbody->velocity.z = vertical * cos - horizontal * sin;

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

	void PlayerMovement::onTriggerEnter(Physics::Collider* collider)
	{
		int test = 0;
	}

	void PlayerMovement::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		gameObject.addComponent<PlayerMovement>();
		auto player = gameObject.getComponent<PlayerMovement>();

		iss >> player->m_speed;
		iss >> player->m_jumpForce;
	}
}