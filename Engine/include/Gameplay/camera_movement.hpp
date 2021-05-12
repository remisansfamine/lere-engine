#pragma once

#include "component.hpp"
#include "transform.hpp"
#include "camera.hpp"

namespace Gameplay
{
	class CameraMovement : public Engine::Component
	{
	private:
		float m_horizontalSpeed = 1.f;
		float m_verticalSpeed = 1.f;
		float yaw = 0.f;
		float pitch = 0.f;

		std::shared_ptr<LowRenderer::Camera> camera = nullptr;
		std::shared_ptr<Physics::Transform> transform = nullptr;
		std::shared_ptr<Physics::Transform> playerTransform = nullptr;

	public:
		CameraMovement(Engine::GameObject& gameObject);

		void start() override;
		void update() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}