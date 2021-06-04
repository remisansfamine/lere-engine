#include "ammo_counter.hpp"

#include "graph.hpp"
#include "time.hpp"

#include "player_shooting.hpp"

namespace Gameplay
{
	AmmoCounter::AmmoCounter(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<AmmoCounter>(this))
	{

	}

	void AmmoCounter::start()
	{
		previousCount = Core::Engine::Graph::findGameObjectWithName("Player")->getComponent<PlayerShooting>()->getMaxAmmoCount();

		for (int i = 0; i < previousCount; i++)
		{
			Engine::GameObject& go = Core::Engine::Graph::instantiate("Ammo", "resources/recipes/ammoSprite.recipe");
			ammoTransforms.push_back(go.getComponent<Physics::Transform>());
			ammoTransforms.back()->m_position.y += 1.f * (float)i;
		}
	}

	void AmmoCounter::update()
	{
		float deltaTime = Core::TimeManager::getDeltaTime();

		for (auto ammoIt = ammoShooted.begin(); ammoIt != ammoShooted.end();)
		{
			auto ammo = *ammoIt;

			ammo->m_position.x += deltaTime * animationSpeed;

			if (ammo->m_position.x > 11.f)
			{
				ammo->m_position.x = initialXPos;
				ammo->getHost().setActive(false);
				ammoIt = ammoShooted.erase(ammoIt);
			}
			else
				ammoIt++;
		}
	}

	void AmmoCounter::reload()
	{
		ammoShooted.clear();

		for (auto ammo : ammoTransforms)
		{
			ammo->m_position.x = initialXPos;
			ammo->getHost().setActive(true);
		}
	}

	void AmmoCounter::updateHud(int ammoCount)
	{
		auto transform = ammoTransforms[ammoCount];

		bool isObjActive = transform->getHost().isActive();
		if (isObjActive)
			ammoShooted.push_back(transform);
	}

	std::string AmmoCounter::toString() const
	{
		return "COMP AMMOCOUNTER ";
	}

	void AmmoCounter::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::shared_ptr<AmmoCounter> ac;
		if (!gameObject.tryGetComponent(ac))
			ac = gameObject.addComponent<AmmoCounter>();
	}
}