#pragma once

#include "component.hpp"

namespace Physics
{
	class Transform;
}

namespace LowRenderer
{
	class Renderer : public Engine::Component
	{
	protected:
		Renderer(Engine::GameObject& gameObject, const std::shared_ptr<Renderer>& childPtr);

		std::shared_ptr<Physics::Transform> m_transform = nullptr;

	public:

		void virtual draw() = 0;
	};
}