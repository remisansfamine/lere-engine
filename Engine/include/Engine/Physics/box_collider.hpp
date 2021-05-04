#pragma once

#include "collider.hpp"

namespace Physics
{
	class BoxCollider : public Collider
	{
	private:
		BoxCollider(Engine::GameObject& gameObject, std::shared_ptr<BoxCollider> ptr);

	public:
		BoxCollider(Engine::GameObject& gameObject);
	};
}