#include "enemy_movement.hpp"

#include "collider.hpp"

namespace Gameplay
{
	EnemyMovement::EnemyMovement(Engine::Entity& owner)
		: EntityMovement(owner)
	{
		m_enemyState = requireComponent<Gameplay::EnemyState>();
		m_transform = m_enemyState->transform;
	}

	void EnemyMovement::fixedUpdate()
	{
		if (m_target)
		{
			auto direction = (m_target->position - m_transform->position).normalized();

			m_rigidbody->velocity.x = direction.x * m_speed;
			m_rigidbody->velocity.z = direction.z * m_speed;

			m_enemyState->isWalking = true;
		}
	}

	void EnemyMovement::onTriggerEnter(Physics::Collider* collider)
	{
		if (collider->getHost().m_name == "Player")
			m_target = collider->getHost().getComponent<Physics::TransformComponent>();
	}

	void EnemyMovement::onTriggerExit(Physics::Collider* collider)
	{
		if (collider->getHost().m_name == "Player")
		{
			m_target = nullptr;
			m_rigidbody->velocity = {0, 0, 0};
			m_enemyState->isWalking = false;
		}
	}

	void EnemyMovement::drawImGui()
	{
		if (ImGui::TreeNode("EnemyMovement"))
		{
			ImGui::DragFloat("MoveSpeed : ", &m_speed);
			ImGui::TreePop();
		}
	}

	std::string EnemyMovement::toString() const
	{
		return "COMP ENEMYMOVEMENT " + std::to_string(m_speed);
	}

	void EnemyMovement::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		EnemyMovement* em;
		if (!owner.tryGetComponent(em))
			em = owner.addComponent<EnemyMovement>();

		iss >> em->m_speed;
		int brak = 0;
	}
}
