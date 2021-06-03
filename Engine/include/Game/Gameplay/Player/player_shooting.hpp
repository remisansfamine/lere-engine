#pragma once

#include <memory>
#include <algorithm> 
#include <imgui.h>

#include "component.hpp"
#include "rigidbody.hpp"
#include "button.hpp"
#include "maths.hpp"
#include "timer.hpp"

namespace Gameplay
{
	class PlayerShooting : public Engine::Component
	{
	private:
		std::shared_ptr<Physics::Transform> m_cameraTransform;
		std::shared_ptr<Physics::Transform> m_weaponTransform;
		Core::Maths::vec3 initRotation;

		int ammo = 5;

		bool isReloading = false;
		bool reload = false;

	public:
		PlayerShooting(Engine::GameObject& gameObject);

		void shooting();
		void reloading();

		void start() override;
		void update() override;
		void drawImGui() override {}

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

		Core::Timer timer;
	};
}

