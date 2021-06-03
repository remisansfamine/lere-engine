#include "player_shooting.hpp"

#include <imgui.h>
#include <irrklang/irrklang.h>

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

irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();

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
			if (ammo <= 0)
			{
				Core::Debug::Log::info("No more ammo, please reload.");
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

				Physics::RaycastHit raycastHit;
				Physics::Ray ray{ m_cameraTransform->getGlobalPosition(), m_cameraTransform->getForward(), 50.f };

				ammo--;

				soundEngine->play2D("resources/sounds/shoot.wav");

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
		std::shared_ptr<PlayerShooting> ps;
		if (!gameObject.tryGetComponent(ps))
			ps = gameObject.addComponent<PlayerShooting>();
	}
}
