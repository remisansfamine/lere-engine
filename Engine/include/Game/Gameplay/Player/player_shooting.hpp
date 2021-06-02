#pragma once

#include <memory>
#include <algorithm> 
#include <imgui.h>

#include "component.hpp"
#include "rigidbody.hpp"
#include "button.hpp"
#include "maths.hpp"

namespace Gameplay
{
	class PlayerShooting : public Engine::Component
	{
	private:
		std::shared_ptr<Physics::Transform> m_cameraTransform;

	public:
		PlayerShooting(Engine::GameObject& gameObject);

		void shooting();

		void start() override;
		void update() override;
		void drawImGui() override {}

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}

