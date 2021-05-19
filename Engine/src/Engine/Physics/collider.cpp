#include "collider.hpp"

#include "imGui.h"

#include <algorithm>

#include "debug.hpp"

#include "utils.hpp"

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

	void Collider::computeCallback(bool hasHit, const Collision& collision)
	{
		auto colliderIt = std::find(m_colliders.begin(), m_colliders.end(), collision.collider);

		bool isInVector = colliderIt != m_colliders.end();


		if (isInVector)
		{
			Core::Debug::Log::info("Normal : " + Utils::vecToStringDebug(collision.normal));

			if (!hasHit)
			{
				m_colliders.erase(colliderIt);
				getHost().callCollisionExit(collision);
				return;
			}

			getHost().callCollisionStay(collision);
			return;
		}

		if (hasHit)
		{
			Core::Debug::Log::info("Normal : " + Utils::vecToStringDebug(collision.normal));

			m_colliders.push_back(collision.collider);
			getHost().callCollisionEnter(collision);
			return;
		}
	}

	void Collider::drawImGui()
	{
	}
}