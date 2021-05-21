#pragma once

#include "component.hpp"
#include "rigidbody.hpp"
#include "maths.hpp"

namespace Gameplay
{
	class EnemyState : public Engine::Component
	{
	private:
		std::shared_ptr<Physics::Transform> playerTransform = nullptr;

	public:
		EnemyState(Engine::GameObject& gameObject);

		bool isIdle = false;
		bool isWalking = false;
		bool isFalling = false;
		bool isGrounded = false;

		float horizontalMove = 0.f;
		float forwardMove = 0.f;

		int colliderCount = 0;

		std::shared_ptr<Physics::Transform> transform = nullptr;
		Core::Maths::vec3 direction = {0.f, 0.f, 0.f};

		void start() override;
		void update() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

		void onCollisionEnter(const Physics::Collision& collision) override;
		void onCollisionExit(const Physics::Collision& collision) override;
	};
}