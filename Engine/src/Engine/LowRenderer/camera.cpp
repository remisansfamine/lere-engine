#include "camera.hpp"

#include "imgui.h"

#include "render_manager.hpp"
#include "inputs_manager.hpp"
#include "application.hpp"
#include "time.hpp"

#include "transform.hpp"

#include "debug.hpp"
#include "utils.hpp"

namespace LowRenderer
{
	Camera::Camera(Engine::Entity& owner)
		: Component(owner)
	{
		m_transform = requireComponent<Physics::TransformComponent>();

		LowRenderer::RenderManager::linkComponent(this);
	}

	Core::Maths::mat4 Camera::getViewMatrix() const
	{
		auto globalPosition = -m_transform->getGlobalPosition();
		auto globalRotation = -m_transform->getGlobalRotation();

		// Get the camera view matrix
		return Core::Maths::rotateZ(globalRotation.z) * Core::Maths::rotateX(globalRotation.x) *
			Core::Maths::rotateY(globalRotation.y) * Core::Maths::translate(globalPosition);
	}

	Core::Maths::mat4 Camera::getProjection() const
	{
		// Get the camera projection using the aspect ration, fov, near and far parameters
		return Core::Maths::perspective(fovY * Core::Maths::DEG2RAD, aspect, near, far);
	}

	Core::Maths::mat4 Camera::getOrthographic() const
	{
		// Get the camera orthographic using the aspect ration, fov, near and far parameters
		return Core::Maths::orthographic(-10.f, 10.f, -10.f, 10.f, -1.f, 10.f);
	}

	Core::Maths::mat4 Camera::getViewProjection() const
	{
		return getProjection() * getViewMatrix();
	}

	Core::Maths::mat4 Camera::getViewOrthographic() const
	{
		return getOrthographic();
	}

	bool setCursor(Core::Maths::vec2& deltaMouse)
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

		return isCursorLock;
	}

	void Camera::fixedUpdate()
	{
		aspect = Core::Application::getAspect();
	}

	void Camera::sendViewProjToProgram(const std::shared_ptr<Resources::ShaderProgram> program)
	{
		program->setUniform("viewProj", getViewProjection().e, true, 1, 1);

		Core::Maths::mat4 model = m_transform->globalModel;
		Core::Maths::vec3 viewPos = Core::Maths::vec3(model.e[3], model.e[7], model.e[11]);

		program->setUniform("viewPos", viewPos.e, true);
	}

	void Camera::sendViewOrthoToProgram(const std::shared_ptr<Resources::ShaderProgram> program)
	{
		program->setUniform("viewOrtho", getViewOrthographic().e, true, 1, 1);
	}

	void Camera::sendProjToProgram(const std::shared_ptr<Resources::ShaderProgram> program)
	{
		program->setUniform("viewProj", getViewProjection().e, true, 1, 1);
	}

	void Camera::drawImGui()
	{
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::DragFloat("Near plane :", &near);
			ImGui::DragFloat("Far plane :", &far);
			ImGui::DragFloat("FOV Y :", &fovY);

			Component::drawImGui();

			ImGui::TreePop();
		}
	}

	std::string Camera::toString() const
	{
		return "COMP CAMERA " + std::to_string(near) + " " + std::to_string(far) + " " + std::to_string(fovY);
	}

	void Camera::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		owner.addComponent<Camera>();
		auto cam = owner.getComponent<Camera>();

		iss >> cam->near;
		iss >> cam->far;
		iss >> cam->fovY;
	}
}