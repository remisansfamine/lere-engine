#pragma once

#include "entity_state.hpp"

#include <memory>

#include "timer.hpp"
#include "rigidbody.hpp"

namespace Gameplay
{
	class PlayerLife;

	class EnemyState : public EntityState
	{
	private:
		Core::Timer attackCooldown;
		PlayerLife* playerLife;
		Physics::Rigidbody* rb = nullptr;

	public:
		EnemyState(Engine::Entity& owner);

		void start() override;
		void update() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);

		void hurtPlayer();
		void onCollisionEnter(const Physics::Collision& collision) override;
		void onCollisionStay(const Physics::Collision& collision) override;
	};
}