#pragma once

#include "component.hpp"

#include "maths.hpp"

namespace Physics
{
	class Transform;
};

namespace LowRenderer
{
	class Camera : public Engine::Component
	{
	private:
		Camera(Engine::GameObject& gameObject, const std::shared_ptr<Camera>& ptr);

		std::shared_ptr<Physics::Transform> m_transform = nullptr;

	public:
		float aspect = 0.f;
		float near = 0.001f;
		float far = 200.f;
		float fovY = 90.f;

		Camera(Engine::GameObject& gameObject);

		Core::Maths::mat4 getViewMatrix() const;
		Core::Maths::mat4 getProjection() const;

		void update() override;
	};
}