#include "sphere_collider.hpp"

namespace Physics
{
	SphereCollider::SphereCollider(Engine::GameObject& gameObject, std::shared_ptr<SphereCollider> ptr)
		: Collider(gameObject, ptr)
	{

	}

	SphereCollider::SphereCollider(Engine::GameObject& gameObject)
		: SphereCollider(gameObject, std::shared_ptr<SphereCollider>(this))
	{

	}
}