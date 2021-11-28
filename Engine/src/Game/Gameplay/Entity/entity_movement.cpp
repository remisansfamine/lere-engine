#include "entity_movement.hpp"

namespace Gameplay
{
	EntityMovement::EntityMovement(Engine::Entity& owner)
		: Component(owner)
	{
		m_rigidbody = requireComponent<Physics::Rigidbody>();
	}
}