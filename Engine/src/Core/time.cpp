#include "time.hpp"

#include <glad/glad.h>

#include "debug.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Core
{
	TimeManager::TimeManager()
	{
		Debug::Log::info("Creating the Time Manager");
	}

	TimeManager::~TimeManager()
	{
		Debug::Log::info("Destroying the Time Manager");
	}

	void TimeManager::computeTime()
	{
		TimeManager* TM = instance();

		TM->lastTime = TM->time;

		TM->time = (float)glfwGetTime();

		TM->unscaledDeltaTime = TM->time - TM->lastTime;

		TM->deltaTime = TM->unscaledDeltaTime * TM->timeScale;
	}

	float TimeManager::getTime()
	{
		return instance()->time;
	}

	// TODO: FIX DELTATIME
	float TimeManager::getDeltaTime()
	{
		return instance()->deltaTime;
	}

	float TimeManager::getFixedDeltaTime()
	{
		return instance()->fixedDeltaTime;
	}

	float TimeManager::getUnscaledDeltaTime()
	{
		return instance()->unscaledDeltaTime;
	}

	void TimeManager::setTimeScale(float value)
	{
		// TODO: clamp to 0 if less than 0
		if (value >= 0.f)
			instance()->timeScale = value;
	}
}