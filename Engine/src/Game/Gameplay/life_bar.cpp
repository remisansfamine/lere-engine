#include "life_bar.hpp"

#include "graph.hpp"
#include "entity_life.hpp"

namespace Gameplay
{
	LifeBar::LifeBar(Engine::Entity& owner)
		: Component(owner)
	{

	}

	void LifeBar::start()
	{
		pivot = getHost().getComponent<Physics::TransformComponent>()->getChild(0);
	}

	void LifeBar::updateSprite(int life, int maxLife)
	{
		if (life < 0)
			return;

		Core::Maths::vec3 scale = pivot->scale;
		scale.x = (float)life / (float)maxLife;

		pivot->scale = scale;
	}

	std::string LifeBar::toString() const
	{
		return "COMP LIFEBAR ";
	}

	void LifeBar::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		LifeBar* lb;
		if (!owner.tryGetComponent(lb))
			lb = owner.addComponent<LifeBar>();
	}
}