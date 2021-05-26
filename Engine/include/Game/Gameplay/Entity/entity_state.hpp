#pragma once

#include <memory>
#include <algorithm> 
#include <imgui.h>

#include "component.hpp"
#include "rigidbody.hpp"
#include "maths.hpp"

#include "inputs_manager.hpp"
#include "button.hpp"
#include "graph.hpp"

namespace Gameplay
{
	class EntityState : public Engine::Component
	{
	public:
		EntityState(Engine::GameObject& gameObject, std::shared_ptr<EntityState> ptr);

		std::shared_ptr<Physics::Transform> transform = nullptr;

		bool isIdle = false;
		bool isWalking = false;
		bool isFalling = false;
		bool isGrounded = false;

		float horizontalMove = 0.f;
		float forwardMove = 0.f;

		int colliderCount = 0;
	};
}