#include "rigidbody.hpp"

#include "time.hpp"

namespace Physics
{
	Rigidbody::Rigidbody(Engine::GameObject& gameObject)
		: Rigidbody(gameObject, std::shared_ptr<Rigidbody>(this))
	{
		m_transform = requireComponent<Transform>();
	}

	Rigidbody::Rigidbody(Engine::GameObject& gameObject, std::shared_ptr<Rigidbody> ptr)
		: Component(gameObject, ptr)
	{

	}

	void Rigidbody::addForce(const Core::Maths::vec3& force)
	{

	}

	Core::Maths::vec3 Rigidbody::getNewPosition()
	{
		return m_transform->m_position + velocity * Core::TimeManager::getFixedDeltaTime();
	}

	void Rigidbody::fixedUpdate()
	{
		// C'ets putain de nice tout ça
		float fixedDeltaTime = Core::TimeManager::getFixedDeltaTime();

		velocity += acceleration * fixedDeltaTime;
		m_transform->m_position += velocity * fixedDeltaTime;
	}
}