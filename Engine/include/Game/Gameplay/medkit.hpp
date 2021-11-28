#pragma once

#include "component.hpp"
#include "transform.hpp"

namespace Gameplay
{
	class MedKit : public Engine::Component
	{
	private:

		int healCount = 2;
		Physics::TransformComponent* transform;

	public:
		MedKit(Engine::Entity& owner);

		void start() override;
		void update() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);

		void onTriggerEnter(Physics::Collider* collider) override;
	};
}