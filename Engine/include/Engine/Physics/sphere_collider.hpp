#pragma once

#include "collider.hpp"

namespace Physics
{
	class SphereCollider : public Collider
	{
	private:
		SphereCollider(Engine::GameObject& gameObject, std::shared_ptr<SphereCollider> ptr);
	public:
		SphereCollider(Engine::GameObject& gameObject);
	};
}