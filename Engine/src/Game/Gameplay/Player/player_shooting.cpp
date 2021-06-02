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
		timer.setDelay(0.2f);
	}

	void PlayerShooting::update()
	{
		shooting();
		timer.update();
	}

	void PlayerShooting::shooting()
	{
		if (Core::Input::InputManager::getMouseButtonDown("LeftClick"))
		{
			if (timer.timerOn())
			{
				timer.reset();

				Core::Debug::Log::info("Pan!");
				Physics::RaycastHit raycastHit;
				Physics::Ray ray{ m_cameraTransform->getGlobalPosition(), m_cameraTransform->getForward(), 50.f };
				if (Physics::PhysicManager::raycast(ray, raycastHit))
				{
					std::shared_ptr<EnemyLife> life;
					if (raycastHit.collider->getHost().tryGetComponent(life))
						life->hurt();
				}
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
