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
	class EntityLife : public Engine::Component
	{
	protected:
		EntityLife(Engine::GameObject& gameObject, std::shared_ptr<EntityLife> ptr);
		
		virtual void kill();

		int life = 10;

	public:
		virtual void hurt(int damage = 1);
	};
}