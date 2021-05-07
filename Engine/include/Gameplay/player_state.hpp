#pragma once

#include "component.hpp"

namespace Gameplay
{
	class PlayerState : public Engine::Component
	{
	private:
	public:
		PlayerState(Engine::GameObject& gameObject);

		bool isWalking = false;
		bool isRunning = false;
		bool isJumping = false;
		bool isFalling = false;

		float horizontalMove = 0.f;
		float forwardMove = 0.f;

		void start() override;
		void update() override;
		void drawImGui() override;
	};
}
