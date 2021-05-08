#include "rigidbody.hpp"

#include "imgui.h"

#include "time.hpp"

namespace Physics
{
	Rigidbody::Rigidbody(Engine::GameObject& gameObject)
		: Rigidbody(gameObject, std::shared_ptr<Rigidbody>(this))
	{
		m_transform = requireComponent<Transform>();
	}

	Rigidbody::Rigidbody(Engine::GameObject& gameObject, std::shared_ptr<Rigidbody> ptr)
		: Component(gameObject, ptr)
	{

	}

	void Rigidbody::addForce(const Core::Maths::vec3& force)
	{
		forceSum += force;
	}

	Core::Maths::vec3 Rigidbody::getNewPosition() const
	{
		return m_transform->m_position + velocity * Core::TimeManager::getFixedDeltaTime();
	}

	void Rigidbody::computeNextPos()
	{
		m_transform->m_position = getNewPosition();
	}

	void Rigidbody::fixedUpdate()
	{
		if (wasInCollision)
			return;

		// Calculate the acceleration
		Core::Maths::vec3 dragForce = (velocity ^ abs(velocity)) * drag * 0.5f;
		acceleration = (forceSum + gravity - dragForce) / mass;
		forceSum = Core::Maths::vec3();

		// Update velocity
		velocity += acceleration * Core::TimeManager::getFixedDeltaTime();
	}

	void Rigidbody::drawImGui()
	{
		if (ImGui::TreeNode("Rigidbody"))
		{
			std::string velocityStr = "Velocity : x = " + std::to_string(velocity.x) + "   y = " + std::to_string(velocity.y) + "   z = " + std::to_string(velocity.z);
			ImGui::Text(velocityStr.c_str());

			std::string velocityMagStr = "Velocity magnitude :" + std::to_string(velocity.magnitude());
			ImGui::Text(velocityMagStr.c_str());

			std::string accelerationStr = "Acceleration :" + std::to_string(acceleration.x) + "   y = " + std::to_string(acceleration.y) + "   z = " + std::to_string(acceleration.z);
			ImGui::Text(accelerationStr.c_str());

			std::string accelerationMagStr = "Acceleration magnitude :" + std::to_string(acceleration.magnitude());
			ImGui::Text(accelerationMagStr.c_str());

			ImGui::DragFloat("Mass :", &mass, 1.f, 0.f, 10000.f);
			ImGui::DragFloat("Drag :", &drag, 1.f, 0.f, 100.f);
			ImGui::DragFloat3("Gravity :", &gravity.x, 1.f);

			std::string isAwakeStr = "IsAwake : " + isAwake ? "True" : "False";
			ImGui::Text(isAwakeStr.c_str());

			ImGui::TreePop();
		}
	}
}