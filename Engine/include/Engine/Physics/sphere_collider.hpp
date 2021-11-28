#pragma once

#include "collider.hpp"
#include "sphere.h"

namespace Physics
{
	class SphereCollider : public Collider
	{
	private:
		SphereCollider(Engine::Entity& owner, std::shared_ptr<SphereCollider> ptr);

		void onDestroy() override;
	public:
		SphereCollider(Engine::Entity& owner);

		void updateShape() override;
		void drawImGui() override;

		std::string toString() const override;

		Sphere sphere;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}