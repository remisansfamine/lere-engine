#include "collider.hpp"

#include "imGui.h"

#include <algorithm>

#include "debug.hpp"

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

	void Collider::computeCallback(bool hasHit, std::shared_ptr<Collider> other)
	{
		auto colliderIt = std::find(m_colliders.begin(), m_colliders.end(), other);

		bool isInVector = colliderIt != m_colliders.end();

		if (isInVector)
		{
			if (!hasHit)
			{
				m_colliders.erase(colliderIt);
				getHost().callCollisionExit(other);
				return;
			}

			getHost().callCollisionStay(other);
			return;
		}

		if (hasHit)
		{
			m_colliders.push_back(other);
			getHost().callCollisionEnter(other);
			return;
		}
	}

	void Collider::drawImGui()
	{
	}
}