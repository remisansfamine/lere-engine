#include "camera.hpp"

#include "render_manager.hpp"
#include "inputs_manager.hpp"
#include "application.hpp"
#include "time.hpp"

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

	Core::Maths::mat4 Camera::getViewProjection() const
	{
		return getProjection() * getViewMatrix();
	}

	// TODO: Remove this
	void setCursor(Core::Maths::vec2& deltaMouse)
	{
		static bool isCursorLock = false;

		// Set the cursor visibility
		if (Core::Input::InputManager::getButtonDown("LockCursor"))
		{
			isCursorLock = !isCursorLock;
			Core::Application::setCursor(isCursorLock);
		}

		if (!isCursorLock)
		{
			deltaMouse.x = 0.f;
			deltaMouse.y = 0.f;
		}
	}

	void Camera::fixedUpdate()
	{
		float deltaTime = Core::TimeManager::getFixedDeltaTime();
		float sensivity = 0.3f * deltaTime;

		aspect = Core::Application::getAspect();

		Core::Maths::vec2 deltaMouse = Core::Input::InputManager::getDeltasMouse();

		setCursor(deltaMouse);

		m_transform->m_rotation.x += deltaMouse.y * sensivity;
		m_transform->m_rotation.y += deltaMouse.x * sensivity;

		float translationSpeed = 2.f * deltaTime;

		float forwardMove = Core::Input::InputManager::getAxis("Vertical");
		float strafeMove = Core::Input::InputManager::getAxis("Horizontal");
		float verticalMove = Core::Input::InputManager::getAxis("UpDown");

		float sin = sinf(m_transform->m_rotation.y), cos = cosf(m_transform->m_rotation.y);

		m_transform->m_position.x += (sin * forwardMove + cos * strafeMove) * translationSpeed;
		m_transform->m_position.z += (sin * strafeMove - cos * forwardMove) * translationSpeed;

		m_transform->m_position.y += verticalMove * translationSpeed;
	}

	void Camera::sendToProgram(const std::shared_ptr<Resources::ShaderProgram> program)
	{
		program->setUniform("viewProj", getViewProjection().e, 1, 1);
		program->setUniform("viewPos", m_transform->m_position.e);
	}
}