#pragma once

#include "entity_state.hpp"

#include <memory>

namespace Gameplay
{
	class EnemyState : public EntityState
	{
	private:
		std::shared_ptr<Physics::Transform> playerTransform = nullptr;

	public:
		EnemyState(Engine::GameObject& gameObject);

		Core::Maths::vec3 direction = {0.f, 0.f, 0.f};

		void update() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

		void onCollisionEnter(const Physics::Collision& collision) override;
		void onCollisionExit(const Physics::Collision& collision) override;
	};
}