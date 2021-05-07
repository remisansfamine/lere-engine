#include "box_collider.hpp"

#include "imgui.h"

#include "physic_manager.hpp"

namespace Physics
{
	BoxCollider::BoxCollider(Engine::GameObject& gameObject, std::shared_ptr<BoxCollider> ptr)
		: Collider(gameObject, ptr)
	{
		PhysicManager::linkComponent(ptr);
	}

	BoxCollider::BoxCollider(Engine::GameObject& gameObject)
		: BoxCollider(gameObject, std::shared_ptr<BoxCollider>(this))
	{
	}

	void BoxCollider::updateShape()
	{
		box.center = m_transform->m_position;
	}

	void BoxCollider::drawImGui()
	{
		if (ImGui::TreeNode("Sphere Collider"))
		{
			ImGui:DragFloat("Radius :", &sphere.radius);
			ImGui:DragFloat3("Center :", &sphere.center.x);

			ImGui::TreePop();
		}
	}
}