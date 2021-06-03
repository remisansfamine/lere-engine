#include "entity_life.hpp"

#include "sound_manager.hpp"

namespace Gameplay
{
	EntityLife::EntityLife(Engine::GameObject& gameObject, std::shared_ptr<EntityLife> ptr, const std::string& _hurtSound, const std::string& _deathSound)
		: Component(gameObject, ptr), hurtSound(_hurtSound), deathSound(_deathSound)
	{

	}

	void EntityLife::hurt(int damage)
	{
		life -= damage;

		if (life <= 0)
		{
			kill();
			return;
		}

		Core::Engine::SoundManager::getSoundEngine()->play2D(hurtSound.c_str());
	}

	void EntityLife::kill()
	{
		Core::Debug::Log::info("Ah tabarnak je suis archi dead lo !");

		Core::Engine::SoundManager::getSoundEngine()->play2D(deathSound.c_str());
	}
}