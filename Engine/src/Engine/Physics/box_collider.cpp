#include "box_collider.hpp"

#include "imgui.h"

#include "physic_manager.hpp"
#include "render_manager.hpp"

#include "collider_renderer.hpp"

namespace Physics
{
	BoxCollider::BoxCollider(Engine::GameObject& gameObject, std::shared_ptr<BoxCollider> ptr)
		: Collider(gameObject, ptr)
	{
		PhysicManager::linkComponent(ptr);
		gameObject.addComponent<LowRenderer::ColliderRenderer>(ptr, "resources/obj/colliders/boxCollider.obj");
		LowRenderer::RenderManager::linkComponent(gameObject.getComponent<LowRenderer::ColliderRenderer>());
	}

	BoxCollider::BoxCollider(Engine::GameObject& gameObject)
		: BoxCollider(gameObject, std::shared_ptr<BoxCollider>(this))
	{
	}

	void BoxCollider::updateShape()
	{
		box.center = Core::Maths::modelMatrixToPosition(m_transform->getGlobalModel()) + m_center;
		extensions = box.size;
	}

	void BoxCollider::drawImGui()
	{
		if (ImGui::TreeNode("Box Collider"))
		{
			ImGui::DragFloat3("Center :", &m_center.x);
			ImGui::DragFloat3("Size :", &box.size.x);
			ImGui::Checkbox("IsTrigger", &isTrigger);
			ImGui::Checkbox("IsDraw", &isDraw);
			//ImGui::DragFloat3("Position offset :", &m_positionOffset.x);

			ImGui::TreePop();
		}
	}
}