#include "player_life.hpp"

namespace Gameplay
{
	PlayerLife::PlayerLife(Engine::GameObject& gameObject)
		: EntityLife(gameObject, std::shared_ptr<PlayerLife>(this))
	{

	}

	void PlayerLife::update()
	{
		if (life <= 0)
			Core::Debug::Log::info("You lose!");
	}

	void PlayerLife::hurt(int damage = 1)
	{
		life -= damage;
		Core::Debug::Log::info(std::to_string(life));
	}
}