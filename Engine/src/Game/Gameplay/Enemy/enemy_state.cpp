#include "enemy_state.hpp"

#include <imgui.h>
#include <algorithm>

#include "rigidbody.hpp"
#include "maths.hpp"

#include "inputs_manager.hpp"
#include "graph.hpp"

namespace Gameplay
{
	EnemyState::EnemyState(Engine::GameObject& gameObject)
		: EntityState(gameObject, std::shared_ptr<EnemyState>(this)) 
	{
		playerTransform = Core::Engine::Graph::findGameObjectWithName("Player")->getComponent<Physics::Transform>();
	}

	void EnemyState::update()
	{
		direction = (playerTransform->m_position - transform->m_position).normalize();

		horizontalMove =  direction.x;
		forwardMove = direction.z;
	}

	void EnemyState::drawImGui()
	{
		if (ImGui::TreeNode("EnemyState"))
		{
			std::string horizontalStr = "Horizontal movement : " + std::to_string(horizontalMove);
			ImGui::Text(horizontalStr.c_str());
			std::string forwardStr = "Forward movement : " + std::to_string(forwardMove);
			ImGui::Text(forwardStr.c_str());
			ImGui::TreePop();
		}
	}

	void EnemyState::onCollisionEnter(const Physics::Collision& collision)
	{
		if (collision.normal.y <= 0.f)
			return;

		colliderCount++;
		isGrounded = true;
	}

	void EnemyState::onCollisionExit(const Physics::Collision& collision)
	{
		colliderCount = std::max(0, colliderCount - 1);

		if (colliderCount == 0)
			isGrounded = false;
	}

	std::string EnemyState::toString() const
	{
		return "COMP ENEMYSTATE " + std::to_string(isIdle) + " " + std::to_string(isWalking)
			+ " " + std::to_string(isFalling)
			+ " " + std::to_string(isGrounded);
	}

	void EnemyState::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<EnemyState>())
			gameObject.addComponent<EnemyState>();

		auto enemy = gameObject.getComponent<EnemyState>();

		iss >> enemy->isIdle;
		iss >> enemy->isWalking;
		iss >> enemy->isFalling;
		iss >> enemy->isGrounded;
	}
}