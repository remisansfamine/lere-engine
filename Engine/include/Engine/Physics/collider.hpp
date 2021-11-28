#pragma once

#include <unordered_map>
#include <unordered_set>

#include "component.hpp"
#include "rigidbody.hpp"
#include "collision.hpp"
#include "maths.hpp"

namespace Physics
{
	class Collider : public Engine::Component
	{
	protected:
		std::unordered_map<Collider*, Collision> m_colliders;
		std::unordered_set<Collider*> m_triggers;
		Core::Maths::vec3 m_positionOffset;

		Collider(Engine::Entity& owner);

	public:
		Core::Maths::vec3 m_center;
		Core::Maths::vec3 extensions;

		Physics::TransformComponent* m_transform = nullptr;
		Physics::Rigidbody* m_rigidbody = nullptr;

		bool isTrigger = false;
		bool isDraw = false;

		bool hasRigidbody();
		bool isRigidbodyAwake();

		void computeCollisionCallback(bool hasHit, const Collision& collision);
		void computeTriggerCallback(bool hasHit, Collider* collider);

		void drawImGui() override;

		virtual void updateShape() = 0;
	};
}