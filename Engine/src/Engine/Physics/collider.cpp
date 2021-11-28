#include "collider.hpp"

#include "imGui.h"

#include <algorithm>

#include "debug.hpp"

#include "utils.hpp"

namespace Physics
{
	Collider::Collider(Engine::Entity& owner)
		: Component(owner)
	{
		m_transform = requireComponent<TransformComponent>();
		m_rigidbody = owner.getComponent<Rigidbody>();
	}

	bool Collider::hasRigidbody()
	{
		return m_rigidbody != nullptr;
	}

	bool Collider::isRigidbodyAwake()
	{
		return hasRigidbody() && m_rigidbody->isAwake;
	}

	void Collider::computeCollisionCallback(bool hasHit, const Collision& collision)
	{
		auto colliderIt = m_colliders.find(collision.collider);
		bool isInMap = colliderIt != m_colliders.end();

		if (isInMap)
		{
			if (!hasHit)
			{
				getHost().callCollisionExit(colliderIt->second);
				m_colliders.erase(colliderIt);
				return;
			}

			getHost().callCollisionStay(collision);
			return;
		}

		if (!hasHit)
			return;

		m_colliders[collision.collider] = collision;
		getHost().callCollisionEnter(collision);
	}

	void Collider::computeTriggerCallback(bool hasHit, Collider* collider)
	{
		auto colliderIt = std::find(m_triggers.begin(), m_triggers.end(), collider);

		bool isInSet = colliderIt != m_triggers.end();

		if (isInSet)
		{
			if (!hasHit)
			{
				m_triggers.erase(colliderIt);
				getHost().callTriggerExit(collider);
				return;
			}

			getHost().callTriggerStay(collider);
			return;
		}

		if (hasHit)
		{
			m_triggers.insert(collider);
			getHost().callTriggerEnter(collider);
			return;
		}
	}

	void Collider::drawImGui()
	{
		Component::drawImGui();

		ImGui::Checkbox("IsTrigger", &isTrigger);
		ImGui::Checkbox("IsDraw", &isDraw);
	}
}