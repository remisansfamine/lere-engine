#include "physic_manager.hpp"

#include <algorithm>

#include "debug.hpp"
#include "time.hpp"
#include "graph.hpp"
#include "intersection.h"

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
		auto boxPartition = std::partition(boxColliders.begin(), boxColliders.begin() + lastBoxRigidbodyIndex, [](auto p) { return p->isRigidbodyAwake(); });

		for (auto boxColliderIt = boxColliders.begin(); boxColliderIt != boxPartition; boxColliderIt++)
		{

		}

		// Sort the part of the colliders linked to a rigidbody
		auto spherePartition = std::partition(sphereColliders.begin(), sphereColliders.begin() + lastSphereRigidbodyIndex, [](auto p) { return p->isRigidbodyAwake(); });

		for (auto sphereColliderIt = sphereColliders.begin(); sphereColliderIt != spherePartition; sphereColliderIt++)
		{
			auto sphereCollider = *sphereColliderIt;

			sphereCollider->m_rigidbody->wasInCollision = false;

			for (auto& boxCollider : boxColliders)
			{
				sphereCollider->updateShape();
				boxCollider->updateShape();

				Core::Debug::Log::info("Sphere new pos : x = " + std::to_string(sphereCollider->m_rigidbody->getNewPosition().x) +
					", y = " + std::to_string(sphereCollider->m_rigidbody->getNewPosition().y) +
					", z = " + std::to_string(sphereCollider->m_rigidbody->getNewPosition().z));

				vec3 interPt, interNormal;
				if (IntersectSphereBox(sphereCollider->sphere,
					sphereCollider->m_rigidbody->getNewPosition(),
					boxCollider->box, interPt, interNormal))
				{
					auto relfect = reflect(sphereCollider->m_rigidbody->velocity, interNormal);

					float diff = (sphereCollider->m_rigidbody->getNewPosition() - interPt).magnitude();

					sphereCollider->m_rigidbody->velocity = relfect.normalized() * diff;

					sphereCollider->m_transform->m_position = sphereCollider->m_rigidbody->getNewPosition();

					sphereCollider->m_rigidbody->wasInCollision = true;

					//sphereCollider->m_transform->m_position = interPt;//+ interNormal.normalize() * sphereCollider->sphere.radius;
					//sphereCollider->m_rigidbody->velocity.y = 0.f;//addForce(interNormal.normalize() * sphereCollider->m_rigidbody->velocity.magnitude());
				}
			}
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

			PM->computeCollisions();

			// Call fixed update for all components
			Core::Engine::Graph::fixedUpdate();
		}
	}
}