#pragma once

#include <memory>

#include "maths.hpp"

namespace Physics
{
	class Collider;

	struct Collision
	{
		std::shared_ptr<Collider> collider;
		Core::Maths::vec3 point;
		Core::Maths::vec3 normal;
	};
}