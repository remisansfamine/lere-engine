#include "timer.hpp"

#include "time.hpp"

namespace Core
{
	bool Timer::TimerOn(float timer)
	{
		float currentTime = timer;
		float deltaTime = TimeManager::getDeltaTime();

		for (currentTime = timer; currentTime >= 0; currentTime -= deltaTime)
		{
			if (currentTime <= 0)
			{
				currentTime = timer;
				return true;
			}
		}
	}
}