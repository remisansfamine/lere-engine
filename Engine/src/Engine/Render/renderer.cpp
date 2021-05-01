#include "renderer.hpp"

#include "render_manager.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	Renderer::Renderer(Engine::GameObject& gameObject, const std::shared_ptr<Renderer>& childPtr)
		: Component(gameObject, childPtr)
	{
		RenderManager::linkComponent(childPtr);

		m_transform = requireComponent<Physics::Transform>();
		m_transform->m_position.z = -2.f;
		m_transform->m_position.y = - 1.f;
	}
}