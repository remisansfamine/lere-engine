#include "camera_movement.hpp"

#include <imgui.h>
#include <algorithm>

#include "inputs_manager.hpp"
#include "time.hpp"
#include "graph.hpp"
#include "maths.hpp"
#include "physic_manager.hpp"

#include "utils.hpp"

namespace Gameplay
{
	CameraMovement::CameraMovement(Engine::Entity& owner)
		: Component(owner)
	{
		camera = requireComponent<LowRenderer::Camera>();
		transform = requireComponent<Physics::TransformComponent>();
	}

	void CameraMovement::start()
	{
		playerTransform = transform->getEntityParent().getComponent<Physics::TransformComponent>();
	}

	void CameraMovement::fixedUpdate()
	{
		Core::Maths::vec3 rotation = transform->rotation;

		rotation.x -= m_sensitivity * Core::TimeManager::getFixedDeltaTime() * Core::Input::InputManager::getDeltasMouse().y;
		rotation.x = std::clamp(rotation.x, -Core::Maths::PIO2, Core::Maths::PIO2);

		transform->rotation = rotation;
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

	void CameraMovement::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		owner.addComponent<CameraMovement>();
		auto player = owner.getComponent<CameraMovement>();

		iss >> player->m_sensitivity;
	}
}