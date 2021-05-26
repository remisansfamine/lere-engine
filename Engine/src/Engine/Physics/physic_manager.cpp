#include "physic_manager.hpp"

#include <algorithm>

#include "debug.hpp"
#include "time.hpp"
#include "graph.hpp"

#include "intersection.h"
#include "utils.hpp"
#include "collision.hpp"

namespace Physics
{
	PhysicManager::PhysicManager()
	{
		Core::Debug::Log::info("Creating the Physic Manager");
	}

	PhysicManager::~PhysicManager()
	{
		Core::Debug::Log::info("Destroying the Physic Manager");
	}

	std::vector<RaycastHit> PhysicManager::raycastAll(const Ray& ray)
	{
		PhysicManager* PM = instance();

		std::vector<RaycastHit> raycastHits;

		vec3 segmentOrigin = ray.origin;
		vec3 segmentEnding = ray.origin + ray.direction.normalized() * ray.distance;

		for (auto& boxCollider : PM->boxColliders)
		{
			boxCollider->updateShape();

			RaycastHit rayHit { boxCollider };

			Box newBox = boxCollider->box;
			newBox.center = boxCollider->m_center;
			newBox.size = boxCollider->extensions;
			newBox.quaternion = Core::Maths::quaternionFromEuler(boxCollider->m_transform->m_rotation);

			if (IntersectSegmentBox(segmentOrigin, segmentEnding, newBox, rayHit.hit))
			{
				rayHit.distance = squaredDistance(ray.origin, rayHit.hit.point);
				raycastHits.push_back(rayHit);
			}
		}

		for (auto& sphereCollider : PM->sphereColliders)
		{
			sphereCollider->updateShape();

			RaycastHit rayHit { sphereCollider };

			Sphere newSphere = sphereCollider->sphere;
			newSphere.center = sphereCollider->m_center;
			newSphere.radius = sphereCollider->extensions.x;
			newSphere.quaternion = Core::Maths::quaternionFromEuler(sphereCollider->m_transform->m_rotation);

			if (IntersectSegmentSphere(segmentOrigin, segmentEnding, newSphere, rayHit.hit))
			{
				rayHit.distance = squaredDistance(ray.origin, rayHit.hit.point);
				raycastHits.push_back(rayHit);
			}
		}

		return raycastHits;
	}

	bool PhysicManager::raycast(const Ray& ray, RaycastHit& raycastHit)
	{
		std::vector<RaycastHit> raycastHits = raycastAll(ray);

		if (raycastHits.empty())
			return false;

		raycastHit = raycastHits[0];

		for (int i = 1; i < raycastHits.size(); i++)
			if (raycastHits[i].distance < raycastHit.distance)
				raycastHit = raycastHits[i];

		return true;
	}

	void PhysicManager::linkComponent(const std::shared_ptr<Rigidbody> compToLink)
	{
		instance()->rigidbodies.push_back(compToLink);
	}

	void PhysicManager::linkComponent(const std::shared_ptr<BoxCollider> compToLink)
	{
		PhysicManager* PM = instance();

		if (compToLink->hasRigidbody())
		{
			PM->boxColliders.insert(PM->boxColliders.begin(), compToLink);
			PM->lastBoxRigidbodyIndex++;
			return;
		}

		PM->boxColliders.insert(PM->boxColliders.end(), compToLink);
	}

	void PhysicManager::linkComponent(const std::shared_ptr<SphereCollider> compToLink)
	{
		PhysicManager* PM = instance();

		if (compToLink->hasRigidbody())
		{
			PM->sphereColliders.insert(PM->sphereColliders.begin(), compToLink);
			PM->lastSphereRigidbodyIndex++;
			return;
		}

		PM->sphereColliders.insert(PM->sphereColliders.end(), compToLink);
	}

	void PhysicManager::computeCollisions()
	{
		// Sort the part of the colliders linked to a rigidbody
		auto spherePartition = std::partition(sphereColliders.begin(), sphereColliders.begin() + lastSphereRigidbodyIndex, [](auto p) { return p->isRigidbodyAwake(); });

		for (auto sphereColliderIt = sphereColliders.begin(); sphereColliderIt != spherePartition; sphereColliderIt++)
		{
			auto sphereCollider = *sphereColliderIt;

			for (auto& boxCollider : boxColliders)
			{
				sphereCollider->updateShape();
				boxCollider->updateShape();

				Sphere newSphere = sphereCollider->sphere;
				newSphere.center = sphereCollider->m_center;
				newSphere.radius = sphereCollider->extensions.x;
				newSphere.quaternion = Core::Maths::quaternionFromEuler(sphereCollider->m_transform->m_rotation);

				Box newBox = boxCollider->box;
				newBox.center = boxCollider->m_center;
				newBox.size = boxCollider->extensions;
				newBox.quaternion = Core::Maths::quaternionFromEuler(boxCollider->m_transform->m_rotation);

				Collision collision = { boxCollider };

				sphereCollider->computeCallback(IntersectSphereBox(newSphere, sphereCollider->m_rigidbody->getNewPosition(newSphere.center), newBox, collision.hit), collision);
			}

			sphereCollider->m_rigidbody->computeNextPos();
		}
	}

	void PhysicManager::update()
	{
		PhysicManager* PM = instance();

		PM->timeStocker += Core::TimeManager::getDeltaTime();

		float fixedDeltaTime = Core::TimeManager::getFixedDeltaTime();

		// Fixed loop
		while (PM->timeStocker >= fixedDeltaTime)
		{
			PM->timeStocker -= fixedDeltaTime;

			// Call fixed update for all components
			Core::Engine::Graph::fixedUpdate();

			PM->computeCollisions();
		}
	}
}