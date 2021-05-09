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

				//Core::Debug::Log::info("Sphere new pos : x = " + std::to_string(sphereCollider->m_rigidbody->getNewPosition().x) +
				//	", y = " + std::to_string(sphereCollider->m_rigidbody->getNewPosition().y) +
				//	", z = " + std::to_string(sphereCollider->m_rigidbody->getNewPosition().z));

				Sphere newSphere = sphereCollider->sphere;
				newSphere.center = sphereCollider->m_center;
				newSphere.radius = sphereCollider->extensions.x;
				newSphere.quaternion = Core::Maths::quaternionFromEuler(sphereCollider->m_transform->m_rotation);

				Box newBox = boxCollider->box;
				newBox.center = boxCollider->m_center;
				newBox.size = boxCollider->extensions;
				newBox.quaternion = Core::Maths::quaternionFromEuler(boxCollider->m_transform->m_rotation);

				vec3 interPt, interNormal;
				if (IntersectSphereBox(newSphere,
					sphereCollider->m_rigidbody->getNewPosition(),
					newBox, interPt, interNormal))
				{
					Core::Maths::vec3 axisY = interNormal.normalized();
					Core::Maths::vec3 axisX = (axisY ^ sphereCollider->m_rigidbody->velocity).normalized();

					Core::Maths::vec3 supportReaction  = axisY * fabsf(dot(sphereCollider->m_rigidbody->velocity, axisY));
					Core::Maths::vec3 tangeantReaction = axisX * fabsf(dot(sphereCollider->m_rigidbody->velocity, axisX));

					sphereCollider->m_rigidbody->velocity += supportReaction + tangeantReaction;
					sphereCollider->m_transform->m_position = interPt;
					//sphereCollider->m_transform->m_position = interPt;//+ interNormal.normalize() * sphereCollider->sphere.radius;

					sphereCollider->computeCallback(true, boxCollider);
					boxCollider->computeCallback(true, sphereCollider);

					continue;
				}

				sphereCollider->computeCallback(false, boxCollider);
				boxCollider->computeCallback(false, sphereCollider);
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