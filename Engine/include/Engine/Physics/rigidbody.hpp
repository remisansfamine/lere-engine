#pragma once

#include <memory>

#include "component.hpp"
#include "maths.hpp"

#include "transform.hpp"

namespace Physics
{
	class Rigidbody : public Engine::Component
	{
	private:
		Rigidbody(Engine::GameObject& gameObject, std::shared_ptr<Rigidbody> ptr);

		Core::Maths::vec3 acceleration = Core::Maths::vec3(0.f, -0.5f, 0.f);

		std::shared_ptr<Transform> m_transform = nullptr;

	public:
		Rigidbody(Engine::GameObject& gameObject);

		Core::Maths::vec3 velocity;
		float mass = 1.f;

		bool isAwake = false;

		void addForce(const Core::Maths::vec3& force);

		void fixedUpdate() override;
	};
}