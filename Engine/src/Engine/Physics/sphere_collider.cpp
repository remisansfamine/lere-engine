#include "sphere_collider.hpp"

#include "imgui.h"

#include "physic_manager.hpp"
#include "render_manager.hpp"

#include "collider_renderer.hpp"

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
		sphere.center = Core::Maths::modelMatrixToPosition(m_transform->getGlobalModel()) + m_center;
		extensions = Core::Maths::vec3(sphere.radius, sphere.radius, sphere.radius);
	}

	void SphereCollider::drawImGui()
	{
		if (ImGui::TreeNode("Sphere Collider"))
		{
			ImGui::DragFloat3("Center :", &m_center.x);
			ImGui::DragFloat("Radius :", &sphere.radius);
			ImGui::Checkbox("IsTrigger", &isTrigger);
			ImGui::Checkbox("IsDraw", &isDraw);
			//ImGui::DragFloat3("Position offset :", &m_positionOffset.x);

			ImGui::TreePop();
		}
	}
}