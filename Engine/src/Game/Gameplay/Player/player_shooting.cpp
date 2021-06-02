#include "player_shooting.hpp"

#include "imgui.h"

#include "inputs_manager.hpp"
#include "physic_manager.hpp"
#include "player_movement.hpp"
#include "game_object.hpp"
#include "entity_life.hpp"
#include "enemy_life.hpp"
#include "graph.hpp"
#include "time.hpp"
#include "timer.hpp"
#include <cmath>

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

		deltaTime = Core::TimeManager::getDeltaTime();
	}

	void PlayerShooting::update()
	{
		shooting();
		reloading();

		timer.update();

		if (ammo <= 0)
			Core::Debug::Log::info("No more ammo, please reload.");

		if (reload)
		{
			m_weaponTransform->m_rotation.z = Core::Maths::lerp(m_weaponTransform->m_rotation.z, Core::Maths::DEG2RAD * 90.f, deltaTime * 2);

			if (m_weaponTransform->m_rotation.z >= Core::Maths::DEG2RAD * 90.f - 0.1f)
				reload = false;
		}
		else
			m_weaponTransform->m_rotation.z = Core::Maths::lerp(m_weaponTransform->m_rotation.z, 0.0f, deltaTime * 2);

	}

	void PlayerShooting::shooting()
	{
		if (Core::Input::InputManager::getMouseButtonDown("LeftClick"))
		{
			if (timer.timerOn() && ammo > 0)
			{
				timer.setDelay(0.2f);

				Core::Debug::Log::info("Pan!");
				Physics::RaycastHit raycastHit;
				Physics::Ray ray{ m_cameraTransform->getGlobalPosition(), m_cameraTransform->getForward(), 50.f };

				ammo--;

				Core::Debug::Log::info(std::to_string(ammo));

				if (Physics::PhysicManager::raycast(ray, raycastHit))
				{
					std::shared_ptr<EnemyLife> life;
					if (raycastHit.collider->getHost().tryGetComponent(life))
						life->hurt();
				}
			}
		}
	}

	void PlayerShooting::reloading()
	{
		if (Core::Input::InputManager::getButtonDown("Reload") && ammo < 5)
		{
			if (timer.timerOn())
			{
				timer.setDelay(3.f);
				ammo = 5;
				reload = true;
			}
		}
	}

	void PlayerShooting::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<PlayerShooting>())
			gameObject.addComponent<PlayerShooting>();

		auto player = gameObject.getComponent<PlayerShooting>();
	}
}
