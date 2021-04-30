#include "transform.hpp"

namespace Physics
{
	Transform::Transform(Engine::GameObject& gameObject)
		: Transform(gameObject, std::shared_ptr<Transform>(this))
	{

	}

	Transform::Transform(Engine::GameObject& gameObject, const std::shared_ptr<Transform>& ptr)
		: Component(gameObject, ptr)
	{
		//gameObject.m_transform = ptr;
	}
}