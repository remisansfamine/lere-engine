#pragma once

#include "entity_state.hpp"

#include <memory>

namespace Gameplay
{
	class EnemyState : public EntityState
	{
	public:
		EnemyState(Engine::GameObject& gameObject);

		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

		void onCollisionEnter(const Physics::Collision& collision) override;
		void onCollisionExit(const Physics::Collision& collision) override;
	};
}