#include "entity_life.hpp"

namespace Gameplay
{
	EntityLife::EntityLife(Engine::GameObject& gameObject, std::shared_ptr<EntityLife> ptr)
		: Component(gameObject, ptr)
	{

	}
}