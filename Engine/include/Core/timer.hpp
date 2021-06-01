#pragma once

#include <memory>
#include <algorithm> 
#include <imgui.h>

#include "component.hpp"
#include "rigidbody.hpp"
#include "button.hpp"
#include "maths.hpp"

namespace Core
{
	class Timer
	{
	public:
		static bool TimerOn(float timer);

		bool finish = false;

		float time = 0.f;
		float currentTime = 0.f;
		bool test = false;

	};
}