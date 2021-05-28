#pragma once

#include "entity_life.hpp"

namespace Gameplay
{
	class PlayerLife : public EntityLife
	{
	private:

	public:
		PlayerLife(Engine::GameObject& gameObject);

		void start() override {}
		void update() override;
		void drawImGui() override {}

		//std::string toString() const override {}

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss) {}

		void onCollisionEnter(const Physics::Collision& collision) override {}
		void onCollisionExit(const Physics::Collision& collision) override {}
	};
}