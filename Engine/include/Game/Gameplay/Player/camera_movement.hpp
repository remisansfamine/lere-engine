#pragma once

#include "component.hpp"
#include "transform.hpp"
#include "camera.hpp"

namespace Gameplay
{
	class CameraMovement : public Engine::Component
	{
	private:
		float m_sensitivity = 1.f;

		float yaw = 0.f;
		float pitch = 0.f;

		LowRenderer::Camera* camera = nullptr;
		Physics::TransformComponent* transform = nullptr;
		Physics::TransformComponent* playerTransform = nullptr;

	public:
		CameraMovement(Engine::Entity& owner);

		void start() override;
		void fixedUpdate() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}