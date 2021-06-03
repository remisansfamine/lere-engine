#pragma once

#include <irrklang/irrklang.h>

#include "singleton.hpp"
#include "maths.hpp"

namespace Core::Engine
{
	class SoundManager final : public Singleton<SoundManager>
	{
		friend class Singleton<SoundManager>;

	private:
		SoundManager();
		~SoundManager();

		irrklang::ISoundEngine* m_soundEngine = nullptr;

	public:
		static void init();
		static irrklang::ISoundEngine* getSoundEngine();
	};
}