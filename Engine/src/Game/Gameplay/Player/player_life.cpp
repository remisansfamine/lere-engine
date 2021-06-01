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
}