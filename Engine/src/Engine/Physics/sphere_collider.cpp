#include "sphere_collider.hpp"

#include "physic_manager.hpp"

namespace Physics
{
	SphereCollider::SphereCollider(Engine::GameObject& gameObject, std::shared_ptr<SphereCollider> ptr)
		: Collider(gameObject, ptr) 
	{
		PhysicManager::linkComponent(ptr);
	}

	SphereCollider::SphereCollider(Engine::GameObject& gameObject)
		: SphereCollider(gameObject, std::shared_ptr<SphereCollider>(this))
	{
		sphere = Sphere(vec3(0.f), 1.f);
	}

	void SphereCollider::updateShape()
	{
		sphere.center = m_transform->m_position;
	}
}