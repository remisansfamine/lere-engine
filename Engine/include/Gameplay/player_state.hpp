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
		bool isGrounded = false;

		float horizontalMove = 0.f;
		float forwardMove = 0.f;

		int colliderCount = 0;

		void start() override;
		void update() override;
		void drawImGui() override;

		std::string toString();

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

		void onCollisionEnter(std::shared_ptr<Physics::Collider> other) override;
		void onCollisionExit(std::shared_ptr<Physics::Collider> other) override;
	};
}
