#pragma once

#include <vector>

#include "component.hpp"
#include "rigidbody.hpp"
#include "maths.hpp"

namespace Physics
{
	class Collider : public Engine::Component
	{
	protected:
		std::vector<std::shared_ptr<Collider>> m_colliders;
		Core::Maths::vec3 m_positionOffset;

		Collider(Engine::GameObject& gameObject, std::shared_ptr<Collider> ptr);

	public:
		Core::Maths::vec3 m_center;
		Core::Maths::vec3 extensions;

		std::shared_ptr<Physics::Transform> m_transform = nullptr;
		std::shared_ptr<Physics::Rigidbody> m_rigidbody = nullptr;

		bool isTrigger = false;
		bool isDraw = false;

		bool hasRigidbody();
		bool isRigidbodyAwake();

		void computeCallback(bool hasHit, std::shared_ptr<Collider> other);

		virtual void updateShape() = 0;

		void drawImGui() override;
	};
}