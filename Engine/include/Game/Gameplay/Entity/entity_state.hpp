#pragma once

#include <memory>

#include "component.hpp"

namespace Physics
{
	class TransformComponent;
}

namespace Gameplay
{
	class EntityState : public Engine::Component
	{
	protected:
		EntityState(Engine::Entity& owner);

	public:
		Physics::TransformComponent* transform = nullptr;

		bool isIdle = false;
		bool isWalking = false;
		bool isRunning = false;
		bool isJumping = false;
		bool isFalling = false;
		bool isGrounded = false;

		float horizontalMove = 0.f;
		float forwardMove = 0.f;

		int colliderCount = 0;
	};
}