#include "box_collider.hpp"

#include "physic_manager.hpp"

namespace Physics
{
	BoxCollider::BoxCollider(Engine::GameObject& gameObject, std::shared_ptr<BoxCollider> ptr)
		: Collider(gameObject, ptr)
	{
		PhysicManager::linkComponent(ptr);
	}

	BoxCollider::BoxCollider(Engine::GameObject& gameObject)
		: BoxCollider(gameObject, std::shared_ptr<BoxCollider>(this))
	{

	}
}