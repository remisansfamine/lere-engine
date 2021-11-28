#include "entity_state.hpp"

#include <imgui.h>

#include "rigidbody.hpp"
#include "maths.hpp"
#include "transform.hpp"

#include "inputs_manager.hpp"

namespace Gameplay
{
	EntityState::EntityState(Engine::Entity& owner)
		: Component(owner)
	{
		transform = requireComponent<Physics::TransformComponent>();
	}
}