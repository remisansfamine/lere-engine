#pragma once

#include "component.hpp"

namespace Physics
{
	class TransformComponent;
}

class NoClipMovement : public Engine::Component
{
private:
	float m_speed = 50.f;
	float m_sensivity = 0.1f;

	float horizontalMove = 0.f;
	float forwardMove = 0.f;
	float verticalMove = 0.f;

	float horizontalRotation = 0.f;
	float verticalRotation = 0.f;

	Physics::TransformComponent* m_transform;

public:
	NoClipMovement(Engine::Entity& owner);
	
	void start() override;
	void fixedUpdate() override;
	void update() override;

	static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
};