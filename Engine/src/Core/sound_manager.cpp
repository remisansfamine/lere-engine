#include "sound_manager.hpp"

#include "debug.hpp"

namespace Core::Engine
{
	SoundManager::SoundManager()
	{
		Core::Debug::Log::info("Creating the Sound Manager");
	}

	SoundManager::~SoundManager()
	{
		Core::Debug::Log::info("Destroying the Sound Manager");
	}

	void SoundManager::init()
	{
		instance()->m_soundEngine = irrklang::createIrrKlangDevice();
	}

	irrklang::ISoundEngine* SoundManager::getSoundEngine()
	{
		return instance()->m_soundEngine;
	}
}