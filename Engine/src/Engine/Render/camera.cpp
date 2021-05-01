#include "camera.hpp"


#include "render_manager.hpp"
#include "inputs_manager.hpp"
#include "transform.hpp"
#include "application.hpp"
#include "time.hpp"

namespace LowRenderer
{
	void getDeltasMouse(GLFWwindow* window, float& deltaX, float& deltaY, float& mouseX, float& mouseY)
	{
		double newMouseX, newMouseY;

		glfwGetCursorPos(window, &newMouseX, &newMouseY);
		deltaX = (float)(newMouseX - mouseX);
		deltaY = (float)(newMouseY - mouseY);
		mouseX = (float)newMouseX;
		mouseY = (float)newMouseY;
	}


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
		return /*Core::Maths::rotateZ(m_transform->m_rotation.z) * */Core::Maths::rotateX(m_transform->m_rotation.x) *
			   Core::Maths::rotateY(m_transform->m_rotation.y) * Core::Maths::translate(-m_transform->m_position);
	}

	Core::Maths::mat4 Camera::getProjection() const
	{
		// Get the camera projection using the aspect ration, fov, near and far parameters
		return Core::Maths::perspective(fovY * Core::Maths::DEG2RAD, aspect, near, far, false);
	}

	void setCursor(Core::Maths::vec2& deltaMouse)
	{
		static bool isCursorLock = false;

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

	void Camera::update()
	{
		float sensivity = 0.008f;

		aspect = Core::Application::getAspect();

		Core::Maths::vec2 deltaMouse;
		Core::Application::getDeltasMouse(deltaMouse);

		setCursor(deltaMouse);

		m_transform->m_rotation.x += deltaMouse.y * sensivity;
		m_transform->m_rotation.y += deltaMouse.x * sensivity;

		float forwardMove = Core::Input::InputManager::getAxis("Vertical") * 0.5f * Core::TimeManager::getDeltaTime();
		float strafeMove = Core::Input::InputManager::getAxis("Horizontal") * 0.5f * Core::TimeManager::getDeltaTime();
		float verticalMove = Core::Input::InputManager::getAxis("UpDown") * 0.5f * Core::TimeManager::getDeltaTime();

		m_transform->m_position.x += sinf(m_transform->m_rotation.y) * forwardMove + cosf(m_transform->m_rotation.y) * strafeMove;
		m_transform->m_position.z -= cosf(m_transform->m_rotation.y) * forwardMove - sinf(m_transform->m_rotation.y) * strafeMove;

		m_transform->m_position.y += verticalMove;
	}
}