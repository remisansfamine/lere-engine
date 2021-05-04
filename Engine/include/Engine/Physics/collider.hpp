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
		std::shared_ptr<Physics::Rigidbody> m_rigidbody = nullptr;
		Core::Maths::vec3 m_center;

		Collider(Engine::GameObject& gameObject, std::shared_ptr<Collider> ptr);

	public:
		bool isTrigger = false;

		bool hasRigidbody();
		bool isRigidbodyAwake();

		void callCollisionEnter();
		void callCollisionStay();
		void callCollisionExit();

		//void callTriggerEnter();
		//void callTriggerStay();
		//void callTriggerExit();
	};
}