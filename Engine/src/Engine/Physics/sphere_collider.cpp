#include "sphere_collider.hpp"

#include "imgui.h"

#include "physic_manager.hpp"
#include "render_manager.hpp"

#include "collider_renderer.hpp"
#include "utils.hpp"

namespace Physics
{
	SphereCollider::SphereCollider(Engine::GameObject& gameObject, std::shared_ptr<SphereCollider> ptr)
		: Collider(gameObject, ptr) 
	{
		PhysicManager::linkComponent(ptr);
		gameObject.addComponent<LowRenderer::ColliderRenderer>(ptr, "resources/obj/colliders/sphereCollider.obj");
		LowRenderer::RenderManager::linkComponent(gameObject.getComponent<LowRenderer::ColliderRenderer>());
	}

	SphereCollider::SphereCollider(Engine::GameObject& gameObject)
		: SphereCollider(gameObject, std::shared_ptr<SphereCollider>(this))
	{
		sphere = Sphere(vec3(0.f), 1.f);
	}

	void SphereCollider::updateShape()
	{
		//Core::Maths::vec3 scale = Core::Maths::modelMatrixToPosition(globalModel);
		//float minScale = std::min(std::min(scale.x, scale.y), scale.z);

		Core::Maths::mat4 globalModel = m_transform->getGlobalModel();
		m_center = Core::Maths::modelMatrixToPosition(globalModel) + sphere.center;
		extensions = Core::Maths::vec3(sphere.radius, sphere.radius, sphere.radius);
	}

	void SphereCollider::drawImGui()
	{
		if (ImGui::TreeNode("Sphere Collider"))
		{
			ImGui::DragFloat3("Center :", &sphere.center.x);
			ImGui::DragFloat("Radius :", &sphere.radius);
			ImGui::Checkbox("IsTrigger", &isTrigger);
			ImGui::Checkbox("IsDraw", &isDraw);
			//ImGui::DragFloat3("Position offset :", &m_positionOffset.x);

			ImGui::TreePop();
		}
	}

	std::string SphereCollider::toString() const
	{
		return "COMP SPHERECOLLIDER " + Utils::vecToStringParsing(sphere.center) + 
										std::to_string(sphere.radius) + " " +
										Utils::quatToStringParsing(sphere.quaternion);
	}

	void SphereCollider::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		gameObject.addComponent<SphereCollider>();
		auto collider = gameObject.getComponent<SphereCollider>();

		iss >> collider->sphere.center.x;
		iss >> collider->sphere.center.y;
		iss >> collider->sphere.center.z;

		iss >> collider->sphere.radius;

		iss >> collider->sphere.quaternion.x;
		iss >> collider->sphere.quaternion.y;
		iss >> collider->sphere.quaternion.z;
		iss >> collider->sphere.quaternion.w;
	}
}