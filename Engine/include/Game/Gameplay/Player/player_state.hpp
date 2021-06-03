#pragma once

#include <memory>

#include "entity_state.hpp"

namespace Gameplay
{
	class PlayerState : public EntityState
	{
	private:
		std::shared_ptr<Physics::Transform> m_transform;

	public:
		PlayerState(Engine::GameObject& gameObject);

		void update() override;
		void fixedUpdate() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}