#include "timer.hpp"

#include "time.hpp"

namespace Core
{
	void Timer::update()
	{
		if (isPass)
			return;

		float deltaTime = TimeManager::getDeltaTime();

		currentTime -= deltaTime;
		isPass = currentTime <= 0.f;

		Debug::Log::info(std::to_string(currentTime));
	}

	void Timer::setDelay(float myTime)
	{
		delay = myTime;
		reset();
	}

	bool Timer::timerOn()
	{
		return isPass;
	}

	void Timer::reset()
	{
		isPass = false;
		currentTime = delay;
	}
}