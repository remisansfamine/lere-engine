#pragma once

#include "maths.hpp"

#include "component.hpp"

#include "game_object.hpp"

#include "model.hpp"

namespace Physics
{
	class Transform : public Engine::Component
	{
	private:
		Transform(Engine::GameObject& gameObject, const std::shared_ptr<Transform>& ptr);

	public:
		Transform(Engine::GameObject& gameObject);

		Core::Maths::vec3 m_position	= Core::Maths::vec3(0.f, 0.f, 0.f);
		Core::Maths::vec3 m_rotation	= Core::Maths::vec3(0.f, 0.f, 0.f);
		Core::Maths::vec3 m_scale		= Core::Maths::vec3(1.f, 1.f, 1.f);

		Core::Maths::mat4 getModel();
		//Core::Maths::mat4 getPushModel();
	};
}