#pragma once

#include <memory>
#include <algorithm> 
#include <imgui.h>

#include "component.hpp"
#include "rigidbody.hpp"
#include "ammo_counter.hpp"
#include "button.hpp"
#include "maths.hpp"
#include "timer.hpp"

#include "game_master.hpp"

namespace Gameplay
{
	class PlayerShooting : public Engine::Component
	{
	private:
		GameMaster* gameMaster = nullptr;

		Physics::TransformComponent* m_cameraTransform = nullptr;
		Physics::TransformComponent* m_weaponTransform = nullptr;
		AmmoCounter* m_ammoCounter;
		Core::Maths::vec3 initRotation;
		Core::Maths::vec3 initPosition;

		int ammo = 7;
		int maxAmmo = 7;

		float speedLerpReload = 3.5f;
		float speedLerpShoot = 10.f;

		float maxShootDistance = 250.f;
		float recoil = 0.2f;

		float fireRate = 0.2f;

		bool isReloading = false;
		bool reload = false;

		void shoot();

	public:
		PlayerShooting(Engine::Entity& owner);

		void shooting();
		void reloading();

		int getMaxAmmoCount();
		void start() override;
		void update() override;
		void drawImGui() override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);

		Core::Timer timer;
	};
}

