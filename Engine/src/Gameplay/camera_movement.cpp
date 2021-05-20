#include "camera_movement.hpp"

#include <imgui.h>
#include <algorithm>

#include "inputs_manager.hpp"
#include "time.hpp"
#include "maths.hpp"
#include "utils.hpp"

namespace Gameplay
{
	CameraMovement::CameraMovement(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<CameraMovement>(this))
	{
		camera = requireComponent<LowRenderer::Camera>();
		transform = requireComponent<Physics::Transform>();
	}

	void CameraMovement::start()
	{
		playerTransform = transform->getGOParent().getComponent<Physics::Transform>();
	}

	void CameraMovement::fixedUpdate()
	{
		Core::Maths::vec2 mouseMovement = m_sensitivity * Core::TimeManager::getFixedDeltaTime() * Core::Input::InputManager::getDeltasMouse();

		Core::Debug::Log::info("Deltamouse : " + Utils::vecToStringDebug(mouseMovement));

		transform->m_rotation += Core::Maths::vec3(mouseMovement.y, mouseMovement.x, 0.f);
		transform->m_rotation.x = std::clamp(transform->m_rotation.x, -Core::Maths::PIO2, Core::Maths::PIO2);

		transform->m_position = Core::Maths::vectorRotate(transform->m_position, Core::Maths::quaternionFromEuler(mouseMovement.y * cos(transform->m_rotation.y), mouseMovement.x, mouseMovement.y * sin(transform->m_rotation.y)));
	}

	void CameraMovement::drawImGui()
	{
		if (ImGui::TreeNode("CameraMovement"))
		{
			ImGui::DragFloat("Sensitivity : ", &m_sensitivity);
			ImGui::Text("Arrows Left/Right : rotate horizontaly");
			ImGui::Text("Arrows  Down/Up   : rotate vertically");
			ImGui::TreePop();
		}
	}

	std::string CameraMovement::toString() const
	{
		return "COMP CAMERAMOVEMENT " + std::to_string(m_sensitivity);
	}

	void CameraMovement::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		gameObject.addComponent<CameraMovement>();
		auto player = gameObject.getComponent<CameraMovement>();

		iss >> player->m_sensitivity;
	}
}