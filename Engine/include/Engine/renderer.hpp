#pragma once

#include "component.hpp"

namespace LowRenderer
{
	class Renderer : public Engine::Component
	{
	protected:
		Renderer(Engine::GameObject& gameObject, const std::shared_ptr<Renderer>& childPtr);

	public:

		void virtual draw() = 0;
	};
}