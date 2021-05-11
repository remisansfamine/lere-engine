#include "player_state.hpp"

#include <algorithm> 
#include <imgui.h>

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

	void PlayerState::onCollisionEnter(const Physics::Collision& collision)
	{

		if (collision.normal.y <= 0.f)
			return;

		colliderCount++;
		isGrounded = true;
	}

	void PlayerState::onCollisionExit(const Physics::Collision& collision)
	{
		colliderCount = std::max(0, colliderCount - 1);

		if (colliderCount == 0)
			isGrounded = false;
	}

	std::string PlayerState::toString()
	{
		return "COMP PLAYERSTATE " + std::to_string(isWalking) + " " + std::to_string(isRunning)
			+ " " + std::to_string(isJumping) + " " + std::to_string(isFalling)
			+ " " + std::to_string(isGrounded);
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
	}
}