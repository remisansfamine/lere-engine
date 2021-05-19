#include "enemies_state.hpp"

#include <algorithm> 
#include <imgui.h>

#include "inputs_manager.hpp"
#include "button.hpp"
#include "graph.hpp"

namespace Gameplay
{
	EnemiesState::EnemiesState(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<EnemiesState>(this))
	{

	}

	void EnemiesState::start()
	{

	}

	void EnemiesState::update()
	{
		if (Core::Input::InputManager::getButtonDown("Jump") && isGrounded)
			isJumping = true;

		horizontalMove = Core::Input::InputManager::getAxis("Horizontal");
		forwardMove = Core::Input::InputManager::getAxis("Forward");
	}

	void EnemiesState::drawImGui()
	{
		if (ImGui::TreeNode("EnemiesState"))
		{
			std::string horizontalStr = "Horizontal movement : " + std::to_string(horizontalMove);
			ImGui::Text(horizontalStr.c_str());
			std::string forwardStr = "Forward movement : " + std::to_string(forwardMove);
			ImGui::Text(forwardStr.c_str());
			ImGui::TreePop();
		}
	}

	void EnemiesState::onCollisionEnter(const Physics::Collision& collision)
	{
		if (collision.normal.y <= 0.f)
			return;

		colliderCount++;
		isGrounded = true;
	}

	void EnemiesState::onCollisionExit(const Physics::Collision& collision)
	{
		colliderCount = std::max(0, colliderCount - 1);

		if (colliderCount == 0)
			isGrounded = false;
	}

	std::string EnemiesState::toString() const
	{
		return "COMP PLAYERSTATE " + std::to_string(isWalking) + " " + std::to_string(isRunning)
			+ " " + std::to_string(isJumping) + " " + std::to_string(isFalling)
			+ " " + std::to_string(isGrounded);
	}

	void EnemiesState::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<EnemiesState>())
			gameObject.addComponent<EnemiesState>();

		auto enemies = gameObject.getComponent<EnemiesState>();

		iss >> enemies->isWalking;
		iss >> enemies->isRunning;
		iss >> enemies->isJumping;
		iss >> enemies->isFalling;
		iss >> enemies->isGrounded;
	}
}