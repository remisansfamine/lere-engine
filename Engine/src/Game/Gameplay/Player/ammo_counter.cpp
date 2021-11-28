#include "ammo_counter.hpp"

#include "graph.hpp"
#include "time.hpp"

#include "player_shooting.hpp"

namespace Gameplay
{
	AmmoCounter::AmmoCounter(Engine::Entity& owner)
		: Component(owner)
	{

	}

	void AmmoCounter::start()
	{
		previousCount = Core::Engine::Graph::findEntityWithName("Player")->getComponent<PlayerShooting>()->getMaxAmmoCount();

		Physics::TransformComponent* transform = getHost().getComponent<Physics::TransformComponent>();
		int childrenCount = transform->getChildrenCount();

		for (int i = 0; i < childrenCount; ++i)
			ammoTransforms.push_back(transform->getChild(i));

		for (size_t i = ammoTransforms.size(); i < previousCount; i++)
		{
			Engine::Entity& entity = Core::Engine::Graph::instantiate("Ammo", "resources/recipes/ammoSprite.recipe");
			ammoTransforms.push_back(entity.getComponent<Physics::TransformComponent>());

			Core::Maths::vec3 position = ammoTransforms.back()->position;
			position.y += 1.f * (float)i;
			ammoTransforms.back()->position = position;
		}
	}

	void AmmoCounter::update()
	{
		float deltaTime = Core::TimeManager::getDeltaTime();

		for (auto ammoIt = ammoShooted.begin(); ammoIt != ammoShooted.end();)
		{
			auto ammo = *ammoIt;

			Core::Maths::vec3 position = ammo->position;
			position.x += deltaTime * animationSpeed;
			ammo->position = position;

			if (position.x > 11.f)
			{
				Core::Maths::vec3 position = ammo->position;
				position.x = initialXPos;
				ammo->position = position;

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
			Core::Maths::vec3 position = ammo->position;
			position.x = initialXPos;
			ammo->position = position;

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

	void AmmoCounter::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		AmmoCounter* ac;
		if (!owner.tryGetComponent(ac))
			ac = owner.addComponent<AmmoCounter>();
	}
}