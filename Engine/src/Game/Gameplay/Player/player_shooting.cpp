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
	PlayerShooting::PlayerShooting(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<PlayerShooting>(this))
	{

	}

	void PlayerShooting::start()
	{
		m_cameraTransform = Core::Engine::Graph::findGameObjectWithName("MainCamera")->getComponent<Physics::Transform>();
		m_weaponTransform = Core::Engine::Graph::findGameObjectWithName("Weapon")->getComponent<Physics::Transform>();
		initRotation = m_weaponTransform->m_rotation;
	}

	void PlayerShooting::update()
	{
		shooting();
		reloading();

		timer.update();

		float deltaTime = Core::TimeManager::getDeltaTime();

		if (reload)
		{
			m_weaponTransform->m_rotation.z = Core::Maths::lerp(m_weaponTransform->m_rotation.z, Core::Maths::DEG2RAD * 90.f, deltaTime * 3.5f);

			if (m_weaponTransform->m_rotation.z >= Core::Maths::DEG2RAD * 90.f - 0.1f)
				reload = false;
		}
		else
			m_weaponTransform->m_rotation.z = Core::Maths::lerp(m_weaponTransform->m_rotation.z, 0.f, deltaTime * 3.5f);
	}
	
	void PlayerShooting::shoot()
	{
		Physics::RaycastHit raycastHit;
		Physics::Ray ray(m_cameraTransform->getGlobalPosition(), m_cameraTransform->getForward(), maxShootDistance);

		ammo--;

		Core::Engine::SoundManager::getSoundEngine()->play2D("resources/sounds/shoot.wav");

		if (Physics::PhysicManager::raycast(ray, raycastHit))
		{
			auto& hole = Core::Engine::Graph::instantiate("BulletHole", "resources/recipes/bulletHole.recipe");
			hole.getComponent<Physics::Transform>()->m_position = raycastHit.hit.point;

			std::shared_ptr<EnemyLife> life;
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
				Core::Engine::SoundManager::getSoundEngine()->play2D("resources/sounds/no_ammo.wav");
				return;
			}

			if (reload)
			{
				Core::Debug::Log::info("You cannot shoot while reloading !");
				return;
			}

			if (timer.timerOn())
			{
				timer.setDelay(0.2f);
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
				Core::Engine::SoundManager::getSoundEngine()->play2D("resources/sounds/reload.wav");
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
			ImGui::DragFloat("MaxShootDistance", &maxShootDistance, 1.f, 0.f, 500.f);
			Component::drawImGui();

			ImGui::TreePop();
		}
	}

	void PlayerShooting::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::shared_ptr<PlayerShooting> ps;
		if (!gameObject.tryGetComponent(ps))
			ps = gameObject.addComponent<PlayerShooting>();
	}
}
