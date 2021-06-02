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

namespace Gameplay
{
	PlayerShooting::PlayerShooting(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<PlayerShooting>(this))
	{

	}

	void PlayerShooting::start()
	{
		m_cameraTransform = Core::Engine::Graph::findGameObjectWithName("MainCamera")->getComponent<Physics::Transform>();
	}

	void PlayerShooting::update()
	{
		shooting();
		reloading();

		timer.update();

		if (ammo <= 0)
			Core::Debug::Log::info("No more ammo, please reload.");
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
		bool reload = false;

		if (Core::Input::InputManager::getButtonDown("Reload"))
		{
			if (timer.timerOn())
			{
				timer.setDelay(3.f);
				ammo = 5;
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
