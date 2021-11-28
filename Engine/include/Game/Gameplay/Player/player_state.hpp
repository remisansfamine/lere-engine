#pragma once

#include <memory>

#include "entity_state.hpp"
#include "sphere_collider.hpp"

namespace Gameplay
{
	class PlayerState : public EntityState
	{
	private:
		Physics::TransformComponent* m_transform;
		Physics::SphereCollider* m_collider;

	public:
		PlayerState(Engine::Entity& owner);

		void start() override;
		void update() override;
		void fixedUpdate() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}