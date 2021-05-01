#include "camera.hpp"

#include "render_manager.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	Camera::Camera(Engine::GameObject& gameObject)
		: Camera(gameObject, std::shared_ptr<Camera>(this))
	{
		m_transform = requireComponent<Physics::Transform>();
	}

	Camera::Camera(Engine::GameObject& gameObject, const std::shared_ptr<Camera>& ptr)
		: Component(gameObject, ptr)
	{
		LowRenderer::RenderManager::linkComponent(ptr);
	}

	Core::Maths::mat4 Camera::getViewMatrix() const
	{
		// Get the camera view matrix
		return Core::Maths::rotateZ(m_transform->m_rotation.z) * Core::Maths::rotateX(m_transform->m_rotation.x) *
			   Core::Maths::rotateY(m_transform->m_rotation.y) * Core::Maths::translate(-m_transform->m_position);
	}

	Core::Maths::mat4 Camera::getProjection() const
	{
		// Get the camera projection using the aspect ration, fov, near and far parameters
		return Core::Maths::perspective(fovY * Core::Maths::DEG2RAD, aspect, near, far, false);
	}
}