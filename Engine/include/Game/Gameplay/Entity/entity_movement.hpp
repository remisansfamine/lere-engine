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
#include "time.hpp"

namespace Gameplay
{
	class EntityMovement : public Engine::Component
	{
	protected:
		float m_speed = 0.f;

		Physics::Rigidbody* m_rigidbody;
		Physics::TransformComponent* m_transform;

	public:
		EntityMovement(Engine::Entity& owner);
	};
}