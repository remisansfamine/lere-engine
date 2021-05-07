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
		box.center = m_transform->m_position + m_positionOffset;
	}

	void BoxCollider::drawImGui()
	{
		if (ImGui::TreeNode("Sphere Collider"))
		{
			ImGui::DragFloat3("Size :", &box.size.x);
			ImGui::DragFloat3("Center :", &box.center.x);
			ImGui::DragFloat3("Position offset :", &m_positionOffset.x);

			ImGui::TreePop();
		}
	}
}