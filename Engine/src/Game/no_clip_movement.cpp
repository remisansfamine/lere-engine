#include "no_clip_movement.hpp"

#include "transform.hpp"
#include "time.hpp"
#include "inputs_manager.hpp"
#include "graph.hpp"

NoClipMovement::NoClipMovement(Engine::Entity& owner)
	: Component(owner)
{
	m_transform = requireComponent<Physics::TransformComponent>();
}

void NoClipMovement::start()
{
	Core::Input::InputManager::addAxis("UpDown", GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE);

	Core::Engine::Graph::setCursorState(false);
}

void NoClipMovement::update()
{
	horizontalMove = Core::Input::InputManager::getAxis("Horizontal");
	forwardMove = Core::Input::InputManager::getAxis("Forward");
	verticalMove = Core::Input::InputManager::getAxis("UpDown");

	horizontalRotation -= Core::Input::InputManager::getDeltasMouse().x;
	verticalRotation -= Core::Input::InputManager::getDeltasMouse().y;
}

void NoClipMovement::fixedUpdate()
{
	float fixedSpeed = m_speed * Core::TimeManager::getFixedDeltaTime();
	float horizontal = horizontalMove;
	float vertical = forwardMove;

	Core::Maths::vec3 rotation = m_transform->rotation;
	rotation.y = m_sensivity * Core::TimeManager::getFixedDeltaTime() * horizontalRotation;
	rotation.x = m_sensivity * Core::TimeManager::getFixedDeltaTime() * verticalRotation;
	m_transform->rotation = rotation;

	float cos = cosf(rotation.y), sin = sinf(rotation.y);

	m_transform->position = m_transform->position + Core::Maths::vec3(horizontal * cos + vertical * sin, verticalMove, vertical * cos - horizontal * sin).normalized() * fixedSpeed;
}

void NoClipMovement::parseComponent(Engine::Entity& owner, std::istringstream& iss)
{
	NoClipMovement* movement;
	if (!owner.tryGetComponent(movement))
		movement = owner.addComponent<NoClipMovement>();
}