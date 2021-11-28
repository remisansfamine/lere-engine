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
			if (!boxCollider->isActive() || boxCollider->isTrigger)
				continue;

			boxCollider->updateShape();

			RaycastHit rayHit { boxCollider };

			Box newBox = boxCollider->box;
			newBox.center = boxCollider->m_center;
			newBox.size = boxCollider->extensions;
			newBox.quaternion = Core::Maths::quaternionFromEuler(boxCollider->m_transform->rotation);

			if (IntersectSegmentBox(segmentOrigin, segmentEnding, newBox, rayHit.hit))
			{
				rayHit.distance = squaredDistance(ray.origin, rayHit.hit.point);
				raycastHits.push_back(rayHit);
			}
		}

		for (auto& sphereCollider : PM->sphereColliders)
		{
			if (!sphereCollider->isActive())
				continue;

			sphereCollider->updateShape();

			RaycastHit rayHit { sphereCollider };

			Sphere newSphere = sphereCollider->sphere;
			newSphere.center = sphereCollider->m_center;
			newSphere.radius = sphereCollider->extensions.x;
			newSphere.quaternion = Core::Maths::quaternionFromEuler(sphereCollider->m_transform->rotation);

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

	void PhysicManager::linkComponent(BoxCollider* compToLink)
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

	void PhysicManager::removeComponent(BoxCollider* compToRemove)
	{
		PhysicManager* PM = instance();

		auto colliderIt = PM->boxColliders.find(compToRemove);

		if (colliderIt == PM->boxColliders.end())
			return;

		if ((*colliderIt)->hasRigidbody())
			PM->lastBoxRigidbodyIndex--;

		PM->boxColliders.erase(colliderIt);
	}

	void PhysicManager::linkComponent(SphereCollider* compToLink)
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

	void PhysicManager::removeComponent(SphereCollider* compToRemove)
	{
		PhysicManager* PM = instance();


		auto colliderIt = PM->sphereColliders.find(compToRemove);

		if (colliderIt == PM->sphereColliders.end())
			return;

		if ((*colliderIt)->hasRigidbody())
			PM->lastSphereRigidbodyIndex--;

		PM->sphereColliders.erase(colliderIt);
	}

	void PhysicManager::clearAll()
	{
		clearComponents<SphereCollider>();
		clearComponents<BoxCollider>();
	}

	void PhysicManager::computeCollisions()
	{

		// Sort the part of the colliders linked to a rigidbody
		for (auto sphereColliderIt = sphereColliders.begin(); sphereColliderIt != sphereColliders.end(); sphereColliderIt++)
		{
			auto sphereCollider = *sphereColliderIt;

			if (!sphereCollider->isRigidbodyAwake())
				continue;

			if (!sphereCollider->isActive())
				continue;

			for (auto sphereToCheckIt = sphereColliders.begin(); sphereToCheckIt != sphereColliders.end(); sphereToCheckIt++)
			{
				// Avoid sphere colliding with itself
				if (!sphereCollider->isActive() || sphereToCheckIt == sphereColliderIt)
					continue;

				auto sphereToCheck = *sphereToCheckIt;

				sphereCollider->updateShape();
				sphereToCheck->updateShape();

				Sphere newSphere = sphereCollider->sphere;
				newSphere.center = sphereCollider->m_center;
				newSphere.radius = sphereCollider->extensions.x;
				newSphere.quaternion = Core::Maths::quaternionFromEuler(sphereCollider->m_transform->rotation);

				Sphere newSphereToCheck = sphereToCheck->sphere;
				newSphereToCheck.center = sphereToCheck->m_center;
				newSphereToCheck.radius = sphereToCheck->extensions.x;
				newSphereToCheck.quaternion = Core::Maths::quaternionFromEuler(sphereToCheck->m_transform->rotation);

				if (sphereCollider->isTrigger || sphereToCheck->isTrigger)
				{
					bool hasHit = TriggerSpheres(newSphere, newSphereToCheck);
					sphereCollider->computeTriggerCallback(hasHit, sphereToCheck);
					sphereToCheck->computeTriggerCallback(hasHit, sphereCollider);
					continue;
				}

				Hit hit;
				bool hasHit = IntersectSpheres(newSphere, sphereCollider->m_rigidbody->getNewPosition(newSphere.center), newSphereToCheck, hit);

				sphereCollider->computeCollisionCallback(hasHit, { sphereToCheck, hit });
				sphereToCheck->computeCollisionCallback(hasHit, { sphereCollider, hit });
			}

			for (auto& boxCollider : boxColliders)
			{
				if (!boxCollider->isActive())
					continue;

				sphereCollider->updateShape();
				boxCollider->updateShape();

				Box newBox = boxCollider->box;
				newBox.center = boxCollider->m_center;
				newBox.size = boxCollider->extensions;
				newBox.quaternion = Core::Maths::quaternionFromEuler(boxCollider->m_transform->rotation);

				Sphere newSphere = sphereCollider->sphere;
				newSphere.center = sphereCollider->m_center;
				newSphere.radius = sphereCollider->extensions.x;
				newSphere.quaternion = Core::Maths::quaternionFromEuler(sphereCollider->m_transform->rotation);

				if (sphereCollider->isTrigger || boxCollider->isTrigger)
				{
					bool hasHit = TriggerSphereBox(newSphere, newBox);
					sphereCollider->computeTriggerCallback(hasHit, boxCollider);
					boxCollider->computeTriggerCallback(hasHit, sphereCollider);
					continue;
				}

				Hit hit;
				bool hasHit = IntersectSphereBox(newSphere, sphereCollider->m_rigidbody->getNewPosition(newSphere.center), newBox, hit);

				sphereCollider->computeCollisionCallback(hasHit, { boxCollider, hit });
				boxCollider->computeCollisionCallback(hasHit, { sphereCollider, hit });
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