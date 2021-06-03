#include "player_state.hpp"

#include <imgui.h>
#include <algorithm>

#include "inputs_manager.hpp"
#include "sound_manager.hpp"

#include "rigidbody.hpp"
#include "rigidbody.hpp"
#include "maths.hpp"


namespace Gameplay
{
	PlayerState::PlayerState(Engine::GameObject& gameObject)
		: EntityState(gameObject, std::shared_ptr<PlayerState>(this))
	{
		m_transform = requireComponent<Physics::Transform>();
	}

	void PlayerState::update()
	{
		if (Core::Input::InputManager::getButtonDown("Jump") && isGrounded)
			isJumping = true;

		horizontalMove = Core::Input::InputManager::getAxis("Horizontal");
		forwardMove = Core::Input::InputManager::getAxis("Forward");
	}

		if (Core::Input::InputManager::getMouseButtonDown("LeftClick"))
			Core::Engine::SoundManager::getSoundEngine()->play2D("resources/sounds/shoot.wav");
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

	std::string PlayerState::toString() const
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