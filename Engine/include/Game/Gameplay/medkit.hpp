#pragma once

#include "component.hpp"
#include "transform.hpp"
#include "maths.hpp"

namespace Gameplay
{
	class MedKit : public Engine::Component
	{
	private:

		std::shared_ptr<Physics::Transform> transform;

		Core::Maths::vec3 initPosition;

		bool animation = false;

		int healCount = 2;

		float maxRotation = Core::Maths::TAU;
		float minRotation = 0;
		float speedLerp = 1.5f;
		float limMin = 1.43f;
		float limMax = 1.950f;

	public:
		MedKit(Engine::GameObject& gameObject);

		void start() override;
		void update() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

		void onTriggerEnter(Physics::Collider* collider) override;
	};
}