#include "collider_renderer.hpp"

#include "imgui.h"

#include "render_manager.hpp"

namespace LowRenderer
{
	ColliderRenderer::ColliderRenderer(Engine::Entity& owner, Physics::Collider* ptr, const std::string& modelFilePath)
		: Renderer(owner, "colliderShader", false), collider(ptr), model(modelFilePath, nullptr)
	{
		LowRenderer::RenderManager::linkComponent(this);
	}

	void ColliderRenderer::draw() const
	{
		Core::Maths::mat4 newModel = getModelCollider();
		model.drawCollider(m_shaderProgram, newModel);
	}

	Core::Maths::mat4 ColliderRenderer::getModelCollider() const
	{
		return Core::Maths::translate(collider->m_center) *
			   Core::Maths::rotateX(m_transform->getRotation().x) *
			   Core::Maths::rotateY(m_transform->getRotation().y) *
			   Core::Maths::rotateZ(m_transform->getRotation().z) *
			   Core::Maths::scale(collider->extensions);
	}

	bool ColliderRenderer::canBeDraw() const
	{
		return collider->isDraw;
	}

	void ColliderRenderer::drawImGui()
	{
		if (ImGui::TreeNode("Collider renderer"))
		{
			Component::drawImGui();

			ImGui::Checkbox("IsDraw", &collider->isDraw);

			ImGui::TreePop();
		}
	}

	void ColliderRenderer::onDestroy()
	{
		Component::onDestroy();

		LowRenderer::RenderManager::removeComponent(this);
	}
}