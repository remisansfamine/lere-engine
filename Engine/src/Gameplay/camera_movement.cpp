#include "camera_movement.hpp"

#include <imgui.h>
#include <algorithm>

#include "inputs_manager.hpp"
#include "time.hpp"
#include "maths.hpp"

namespace Gameplay
{
	CameraMovement::CameraMovement(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<CameraMovement>(this))
	{
	}

	void CameraMovement::start()
	{
		camera = getHost().getComponent<LowRenderer::Camera>();
		transform = getHost().getComponent<Physics::Transform>();
		playerTransform = transform->getGOParent().getComponent<Physics::Transform>();
	}

	void CameraMovement::update()
	{
        yaw = Core::Input::InputManager::getAxis("MoveCamSides") * m_horizontalSpeed * Core::TimeManager::getDeltaTime();
		pitch = -Core::Input::InputManager::getAxis("MoveCamUpDown") * m_verticalSpeed * Core::TimeManager::getDeltaTime();

		transform->m_rotation -= Core::Maths::vec3(pitch, yaw, 0.f);
		transform->m_position = Core::Maths::vectorRotate(transform->m_position, Core::Maths::quaternionFromEuler(pitch * cos(transform->m_rotation.y), yaw, pitch * sin(transform->m_rotation.y)));
	}

	void CameraMovement::drawImGui()
	{
		if (ImGui::TreeNode("CameraMovement"))
		{
			ImGui::DragFloat("Horizontal rotation speed : ", &m_horizontalSpeed);
			ImGui::DragFloat("Vertical rotation speed : ", &m_verticalSpeed);
			ImGui::Text("Arrows Left/Right : rotate horizontaly");
			ImGui::Text("Arrows  Down/Up   : rotate vertically");
			ImGui::TreePop();
		}
	}

	std::string CameraMovement::toString() const
	{
		return "COMP CAMERAMOVEMENT " + std::to_string(m_horizontalSpeed) + " " + std::to_string(m_verticalSpeed);
	}

	void CameraMovement::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		gameObject.addComponent<CameraMovement>();
		auto player = gameObject.getComponent<CameraMovement>();

		iss >> player->m_horizontalSpeed;
		iss >> player->m_verticalSpeed;
	}
}