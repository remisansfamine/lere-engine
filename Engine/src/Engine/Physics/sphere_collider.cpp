#include "sphere_collider.hpp"

#include "imgui.h"

#include "physic_manager.hpp"
#include "render_manager.hpp"

#include "collider_renderer.hpp"
#include "utils.hpp"

namespace Physics
{
	SphereCollider::SphereCollider(Engine::Entity& owner)
		: Collider(owner)
	{
		sphere = Sphere(vec3(0.f), 1.f);

		PhysicManager::linkComponent(this);
		owner.addComponent<LowRenderer::ColliderRenderer>(this, "resources/obj/colliders/sphereCollider.obj");
	}

	void SphereCollider::updateShape()
	{
		//Core::Maths::vec3 scale = Core::Maths::modelMatrixToPosition(globalModel);
		//float minScale = std::min(std::min(scale.x, scale.y), scale.z);

		Core::Maths::mat4 globalModel = m_transform->getGlobalModel();
		m_center = Core::Maths::modelMatrixToPosition(globalModel) + sphere.center;
		extensions = Core::Maths::vec3(sphere.radius, sphere.radius, sphere.radius);
	}

	void SphereCollider::onDestroy()
	{
		Component::onDestroy();

		PhysicManager::removeComponent(this);
	}

	void SphereCollider::drawImGui()
	{
		if (ImGui::TreeNode("Sphere Collider"))
		{
			Collider::drawImGui();

			ImGui::DragFloat3("Center :", &sphere.center.x);
			ImGui::DragFloat("Radius :", &sphere.radius);

			ImGui::TreePop();
		}
	}

	std::string SphereCollider::toString() const
	{
		return "COMP SPHERECOLLIDER " + Utils::vecToStringParsing(sphere.center) + 
										std::to_string(sphere.radius) + " " +
										Utils::quatToStringParsing(sphere.quaternion) + std::to_string(isTrigger);
	}

	void SphereCollider::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		SphereCollider* collider;
		if (!owner.tryGetComponent(collider))
			collider = owner.addComponent<SphereCollider>();

		iss >> collider->sphere.center.x;
		iss >> collider->sphere.center.y;
		iss >> collider->sphere.center.z;

		iss >> collider->sphere.radius;

		iss >> collider->sphere.quaternion.x;
		iss >> collider->sphere.quaternion.y;
		iss >> collider->sphere.quaternion.z;
		iss >> collider->sphere.quaternion.w;

		iss >> collider->isTrigger;
	}
}