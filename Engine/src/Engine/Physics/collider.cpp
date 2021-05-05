#include "collider.hpp"

namespace Physics
{
	Collider::Collider(Engine::GameObject& gameObject, std::shared_ptr<Collider> ptr)
		: Component(gameObject, ptr)
	{
		m_transform = requireComponent<Transform>();
		m_rigidbody = gameObject.getComponent<Rigidbody>();
	}

	bool Collider::hasRigidbody()
	{
		return m_rigidbody != nullptr;
	}

	bool Collider::isRigidbodyAwake()
	{
		return hasRigidbody() && m_rigidbody->isAwake;
	}

	void Collider::callCollisionEnter()
	{
		getHost().callCollisionEnter();
	}

	void Collider::callCollisionStay()
	{
		getHost().callCollisionStay();
	}

	void Collider::callCollisionExit()
	{
		getHost().callCollisionExit();
	}
}