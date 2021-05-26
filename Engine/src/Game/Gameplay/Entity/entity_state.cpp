#include "entity_state.hpp"

namespace Gameplay
{
	EntityState::EntityState(Engine::GameObject& gameObject, std::shared_ptr<EntityState> ptr)
		: Component(gameObject, ptr)
	{
		transform = requireComponent<Physics::Transform>();
	}
}