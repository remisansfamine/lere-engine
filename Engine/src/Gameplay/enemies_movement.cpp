#include "enemies_movement.hpp"

#include "imgui.h"

#include "inputs_manager.hpp"
#include "graph.hpp"
#include "time.hpp"

namespace Gameplay
{
	EnemiesMovement::EnemiesMovement(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<EnemiesMovement>(this))
	{
		m_rigidbody = requireComponent<Physics::Rigidbody>();
		m_enemiesState = requireComponent<Gameplay::EnemiesState>();
	}

	void EnemiesMovement::start()
	{
		//m_cameraTransform = Core::Engine::Graph::findGameObjectWithName("MainCamera")->getComponent<Physics::Transform>();
	}

	void EnemiesMovement::fixedUpdate()
	{
		float fixedSpeed = m_speed * Core::TimeManager::getFixedDeltaTime();
		float horizontal = m_enemiesState->horizontalMove * fixedSpeed;
		float vertical = -m_enemiesState->forwardMove * fixedSpeed;

		//m_rigidbody->velocity.x = horizontal * cos(m_cameraTransform->m_rotation.y) + vertical * sin(m_cameraTransform->m_rotation.y);
		//m_rigidbody->velocity.z = -vertical * cos(m_cameraTransform->m_rotation.y) + horizontal * sin(m_cameraTransform->m_rotation.y);		
		
		m_rigidbody->velocity.x = horizontal + vertical;
		m_rigidbody->velocity.z = -vertical + horizontal;

		if (m_enemiesState->isGrounded && m_enemiesState->isJumping)
		{
			m_rigidbody->addForce(Core::Maths::vec3(0.f, m_jumpForce, 0.f));
			m_enemiesState->isJumping = false;
		}
	}

	void EnemiesMovement::drawImGui()
	{
		if (ImGui::TreeNode("EnemiesMovement"))
		{
			ImGui::DragFloat("MoveSpeed : ", &m_speed);
			ImGui::DragFloat("JumpForce : ", &m_jumpForce);
			ImGui::TreePop();
		}
	}

	std::string EnemiesMovement::toString() const
	{
		return "COMP PLAYERMOVEMENT " + std::to_string(m_speed) + " " + std::to_string(m_jumpForce);
	}

	void EnemiesMovement::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		gameObject.addComponent<EnemiesMovement>();
		auto enemies = gameObject.getComponent<EnemiesMovement>();

		iss >> enemies->m_speed;
		iss >> enemies->m_jumpForce;
	}
}
