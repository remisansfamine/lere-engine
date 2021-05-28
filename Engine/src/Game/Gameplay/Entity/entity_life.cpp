#include "entity_life.hpp"

namespace Gameplay
{
	EntityLife::EntityLife(Engine::GameObject& gameObject, std::shared_ptr<EntityLife> ptr)
		: Component(gameObject, ptr)
	{

	}

	void EntityLife::hurt(int damage)
	{
		life -= damage;

		if (life <= 0)
			kill();
	}

	void EntityLife::kill()
	{
		Core::Debug::Log::info("Ah tabarnak je suis archi dead lo !");
	}
}