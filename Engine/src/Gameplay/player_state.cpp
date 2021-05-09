#include "player_state.hpp"

#include "imgui.h"

#include "inputs_manager.hpp"

namespace Gameplay
{
	PlayerState::PlayerState(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<PlayerState>(this))
	{

	}

	void PlayerState::start()
	{

	}

	void PlayerState::update()
	{
		if (Core::Input::InputManager::getButtonDown("Jump") && isGrounded)
			isJumping = true;

		horizontalMove = Core::Input::InputManager::getAxis("Horizontal");
		forwardMove = Core::Input::InputManager::getAxis("Forward");
	}

	void PlayerState::drawImGui()
	{
		if (ImGui::TreeNode("PlayerState"))
		{
			std::string horizontalStr = "Horizontal movement : " + std::to_string(horizontalMove);
			ImGui::Text(horizontalStr.c_str());
			std::string forwardStr = "Forward movement : " + std::to_string(forwardMove);
			ImGui::Text(forwardStr.c_str());
			ImGui::TreePop();
		}
	}

	void PlayerState::onCollisionEnter(std::shared_ptr<Physics::Collider> other)
	{
		isGrounded = true;
		colliderCount++;
	}

	void PlayerState::onCollisionExit(std::shared_ptr<Physics::Collider> other)
	{
		colliderCount--;

		if (colliderCount == 0)
			isGrounded = false;
	}

	std::string PlayerState::toString()
	{
		return "COMP PLAYERSTATE " + std::to_string(isWalking) + " " + std::to_string(isRunning)
			+ " " + std::to_string(isJumping) + " " + std::to_string(isFalling)
			+ " " + std::to_string(isGrounded) + " " + std::to_string(horizontalMove)
			+ " " + std::to_string(forwardMove) + " " + std::to_string(colliderCount);
	}

	void PlayerState::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<PlayerState>())
			gameObject.addComponent<PlayerState>();

		auto player = gameObject.getComponent<PlayerState>();

		iss >> player->isWalking;
		iss >> player->isRunning;
		iss >> player->isJumping;
		iss >> player->isFalling;
		iss >> player->isGrounded;

		iss >> player->horizontalMove;
		iss >> player->forwardMove;

		iss >> player->colliderCount;
	}
}