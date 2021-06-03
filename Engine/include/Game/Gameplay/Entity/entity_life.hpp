#pragma once

#include <memory>
#include <algorithm> 
#include <imgui.h>
#include <string>

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
		EntityLife(Engine::GameObject& gameObject, std::shared_ptr<EntityLife> ptr, const std::string& _hurtSound = "", const std::string& _deathSound = "");
		
		virtual void kill();

		std::string hurtSound = "";
		std::string deathSound = "";

		int life = 10;

	public:
		virtual void hurt(int damage = 1);
	};
}