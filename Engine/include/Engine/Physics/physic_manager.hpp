#pragma once

#include <vector>

#include "singleton.hpp"

#include "rigidbody.hpp"
#include "box_collider.hpp"
#include "sphere_collider.hpp"

namespace Physics
{
	class PhysicManager final : public Singleton<PhysicManager>
	{
		friend Singleton<PhysicManager>;

	private:
		PhysicManager();
		~PhysicManager();

		std::vector<std::shared_ptr<Rigidbody>> rigidbodies;
		std::vector<std::shared_ptr<BoxCollider>> boxColliders;
		std::vector<std::shared_ptr<SphereCollider>> sphereColliders;

		void computeCollisions();
		float timeStocker = 0.f;
		int lastBoxRigidbodyIndex = 0;
		int lastSphereRigidbodyIndex = 0;

	public:


		static void linkComponent(const std::shared_ptr<Rigidbody> compToLink);

		static void linkComponent(const std::shared_ptr<SphereCollider> compToLink);

		static void linkComponent(const std::shared_ptr<BoxCollider> compToLink);

		template <class C>
		static void clearComponents();

		template<>
		static void clearComponents<Rigidbody>()
		{
			instance()->rigidbodies.clear();
		}

		template<>
		static void clearComponents<SphereCollider>()
		{
			instance()->sphereColliders.clear();
			instance()->lastSphereRigidbodyIndex = 0;
		}

		template<>
		static void clearComponents<BoxCollider>()
		{
			instance()->boxColliders.clear();
			instance()->lastBoxRigidbodyIndex = 0;
		}

		static void update();
	};
}