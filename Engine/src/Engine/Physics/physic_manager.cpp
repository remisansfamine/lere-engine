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

				sphereCollider->computeCallback(IntersectSphereBox(newSphere, sphereCollider->m_rigidbody->getNewPosition(), newBox, collision.point, collision.normal), collision);
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