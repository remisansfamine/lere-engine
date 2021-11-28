#pragma once

#include "collider.hpp"
#include "box.h"

namespace Physics
{
	class BoxCollider : public Collider
	{
	private:
		BoxCollider(Engine::Entity& owner, std::shared_ptr<BoxCollider> ptr);

		void onDestroy() override;
	public:
		BoxCollider(Engine::Entity& owner);

		Box box = Box();

		void updateShape() override;

		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}