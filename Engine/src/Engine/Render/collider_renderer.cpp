#include "collider_renderer.hpp"

#include "resources_manager.hpp"

namespace LowRenderer
{
	ColliderRenderer::ColliderRenderer(Engine::GameObject& gameObject, std::shared_ptr<Physics::Collider> ptr, const std::string& modelFilePath)
		: Renderer(gameObject, std::shared_ptr<ColliderRenderer>(this), "colliderShader", false), collider(ptr), model(modelFilePath, nullptr)
	{
	}

	void ColliderRenderer::draw()
	{
		Core::Maths::mat4 newModel = getModelCollider();
		model.drawCollider(m_shaderProgram, newModel);
	}

	Core::Maths::mat4 ColliderRenderer::getModelCollider()
	{
		return m_transform->getGlobalModel() * Core::Maths::translate(collider->m_center) * Core::Maths::scale(collider->extensions);
	}

	bool ColliderRenderer::canBeDraw()
	{
		return collider->isDraw;
	}
}