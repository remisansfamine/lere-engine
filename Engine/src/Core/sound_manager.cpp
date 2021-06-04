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

	void SoundManager::play2D(const std::string& path)
	{
		irrklang::ISoundEngine* soundEngine = instance()->m_soundEngine;

		if (soundEngine)
			soundEngine->play2D(path.c_str());
	}
}