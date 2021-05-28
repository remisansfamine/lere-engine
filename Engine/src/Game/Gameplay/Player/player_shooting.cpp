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
	}

	void PlayerShooting::shooting()
	{
		if (Core::Input::InputManager::getMouseButtonDown("LeftClick"))
		{
			Physics::RaycastHit raycastHit;
			Physics::Ray ray{ m_cameraTransform->getGlobalPosition(), m_cameraTransform->getForward(), 50.f };
			if (Physics::PhysicManager::raycast(ray, raycastHit))
			{
				std::shared_ptr<EnemyLife> life;
				if (raycastHit.collider->getHost().tryGetComponent(life))
				{
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
