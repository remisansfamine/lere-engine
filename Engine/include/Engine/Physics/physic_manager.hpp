#pragma once

#include <unordered_set>

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

		std::unordered_set<BoxCollider*> boxColliders;
		std::unordered_set<SphereCollider*> sphereColliders;

		void computeCollisions();
		float timeStocker = 0.f;
		int lastBoxRigidbodyIndex = 0;
		int lastSphereRigidbodyIndex = 0;

	public:

		static bool raycast(const Ray& ray, RaycastHit& raycastHit);
		static std::vector<RaycastHit> raycastAll(const Ray& ray);

		static void linkComponent(SphereCollider* compToLink);
		static void removeComponent(SphereCollider* compToRemove);

		static void linkComponent(BoxCollider* compToLink);
		static void removeComponent(BoxCollider* compToRemove);

		static void clearAll();

		template <class C>
		static void clearComponents();

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