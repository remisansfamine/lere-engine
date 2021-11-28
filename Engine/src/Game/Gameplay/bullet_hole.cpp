#include "bullet_hole.hpp"

namespace Gameplay
{
	BulletHole::BulletHole(Engine::Entity& owner)
		: Component(owner)
	{

	}

	void BulletHole::start()
	{
		lifeTime.setDelay(1.f);
	}

	void BulletHole::update()
	{
		lifeTime.update();

		if (lifeTime.timerOn())
			getHost().destroy();
	}

	void BulletHole::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		BulletHole* bulletHole;
		if (!owner.tryGetComponent(bulletHole))
			owner.addComponent<BulletHole>();
	}
}