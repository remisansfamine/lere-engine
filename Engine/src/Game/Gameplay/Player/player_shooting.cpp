#include "player_shooting.hpp"

#include <imgui.h>

#include "inputs_manager.hpp"
#include "sound_manager.hpp"
#include "physic_manager.hpp"
#include "graph.hpp"
#include "time.hpp"

#include "player_movement.hpp"
#include "enemy_life.hpp"
#include "timer.hpp"

namespace Gameplay
{
	PlayerShooting::PlayerShooting(Engine::Entity& owner)
		: Component(owner)
	{

	}

	void PlayerShooting::start()
	{
		gameMaster = Core::Engine::Graph::findEntityWithName("GameMaster")->getComponent<GameMaster>();

		m_cameraTransform = Core::Engine::Graph::findEntityWithName("MainCamera")->getComponent<Physics::TransformComponent>();
		m_weaponTransform = Core::Engine::Graph::findEntityWithName("Weapon")->getComponent<Physics::TransformComponent>();
		initRotation = m_weaponTransform->rotation;
		initPosition = m_weaponTransform->position;

		m_ammoCounter = Core::Engine::Graph::findEntityWithName("AmmoCounter")->getComponent<AmmoCounter>();
	}

	int PlayerShooting::getMaxAmmoCount()
	{
		return maxAmmo;
	}

	void PlayerShooting::update()
	{
		if (gameMaster->isPaused)
			return;

		shooting();
		reloading();

		timer.update();

		float deltaTime = Core::TimeManager::getDeltaTime();

		if (reload)
		{

			Core::Maths::vec3 rotation = m_weaponTransform->rotation;
			rotation.z = Core::Maths::lerp(rotation.z, Core::Maths::DEG2RAD * 90.f, deltaTime * speedLerpReload);
			m_weaponTransform->rotation = rotation;

			if (m_weaponTransform->getRotation().z >= Core::Maths::DEG2RAD * 90.f - 0.1f)
			{
				reload = false;
				m_ammoCounter->reload();
			}
		}
		else
			m_weaponTransform->rotation = Core::Maths::lerp((Core::Maths::vec3)m_weaponTransform->rotation, initRotation, deltaTime * speedLerpShoot);

		m_weaponTransform->position = Core::Maths::lerp((Core::Maths::vec3)m_weaponTransform->position, initPosition, deltaTime * speedLerpShoot);
	}
	
	void PlayerShooting::shoot()
	{
		Core::Maths::vec3 rotation = m_weaponTransform->rotation;
		rotation.z += recoil;
		m_weaponTransform->rotation = rotation;

		Core::Maths::vec3 position = m_weaponTransform->position;
		position.z -= recoil;
		m_weaponTransform->position = position;

		ammo--;

		m_ammoCounter->updateHud(ammo);

		Physics::RaycastHit raycastHit;
		Physics::Ray ray(m_cameraTransform->getGlobalPosition(), m_cameraTransform->getForward(), maxShootDistance);

		Core::Engine::SoundManager::play2D("resources/sounds/shoot.wav");

		if (Physics::PhysicManager::raycast(ray, raycastHit))
		{
			auto& hole = Core::Engine::Graph::instantiate("BulletHole", "resources/recipes/bulletHole.recipe");
			hole.getComponent<Physics::TransformComponent>()->position = raycastHit.hit.point;

			EnemyLife* life;
			if (raycastHit.collider->getHost().tryGetComponent(life))
				life->hurt();
		}
	}

	void PlayerShooting::shooting()
	{
		if (Core::Input::InputManager::getMouseButtonDown("LeftClick"))
		{
			if (ammo <= 0)
			{
				Core::Engine::SoundManager::play2D("resources/sounds/no_ammo.wav");
				return;
			}

			if (reload)
			{
				Core::Debug::Log::info("You cannot shoot while reloading !");
				return;
			}

			if (timer.timerOn())
			{
				timer.setDelay(fireRate);
				shoot();
			}
		}
	}

	void PlayerShooting::reloading()
	{
		if (Core::Input::InputManager::getButtonDown("Reload") && ammo < maxAmmo)
		{
			if (timer.timerOn())
			{
				Core::Engine::SoundManager::play2D("resources/sounds/reload.wav");
				timer.setDelay(2.f);
				ammo = maxAmmo;
				reload = true;
			}
		}
	}

	void PlayerShooting::drawImGui()
	{
		if (ImGui::TreeNode("Player Shooting"))
		{
			ImGui::DragFloat("Max shoot distance", &maxShootDistance, 1.f, 0.f, 500.f);
			ImGui::DragFloat("Speed lerp reload", &speedLerpReload, 0.1f, 0.f, 50.f);
			ImGui::DragFloat("Speed lerp shooting", &speedLerpShoot, 0.1f, 0.f, 50.f);
			ImGui::DragFloat("Firerate", &fireRate, 0.01f, 0.f, 5.f);
			Component::drawImGui();

			ImGui::TreePop();
		}
	}

	void PlayerShooting::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		PlayerShooting* ps;
		if (!owner.tryGetComponent(ps))
			ps = owner.addComponent<PlayerShooting>();
	}
}
