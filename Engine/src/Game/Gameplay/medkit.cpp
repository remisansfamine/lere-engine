#include "medkit.hpp"

#include <imgui.h>
#include <algorithm>

#include "player_life.hpp"
#include "collider.hpp"
#include "utils.hpp"
#include "time.hpp"

namespace Gameplay
{
	MedKit::MedKit(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<MedKit>(this))
	{

	}

	void MedKit::start()
	{
		transform = getHost().getComponent<Physics::Transform>();
		initPosition = transform->m_position;
	}

	void MedKit::update()
	{
		float deltaTime = Core::TimeManager::getDeltaTime();

		transform->m_rotation = Utils::clampLoop(transform->m_rotation, 0.f, Core::Maths::TAU);
		transform->m_rotation.y -= deltaTime * speedLerp;

		if (!animation)
		{
			transform->m_position.y = Core::Maths::lerp(transform->m_position.y, 1.5f, deltaTime);

			if (transform->m_position.y <= limMin)
				animation = true;
		}
		else
		{
			transform->m_position.y = Core::Maths::lerp(transform->m_position.y, initPosition.y, deltaTime);
			
			if (transform->m_position.y >= limMax)
			{
				animation = false;
			}
		}
	}

	void MedKit::drawImGui()
	{
		if (ImGui::TreeNode("MedKit"))
		{
			Component::drawImGui();
			ImGui::DragInt("Heal count", &healCount);

			ImGui::TreePop();
		}
	}

	std::string MedKit::toString() const
	{
		return "COMP MEDKIT " + std::to_string(healCount);
	}

	void MedKit::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::shared_ptr<MedKit> mk;
		if (!gameObject.tryGetComponent(mk))
			mk = gameObject.addComponent<MedKit>();

		iss >> mk->healCount;
	}

	void MedKit::onTriggerEnter(Physics::Collider* collider)
	{
		if (collider->getHost().m_name == "Player")
		{
			auto playerLife = collider->getHost().getComponent<PlayerLife>();
			if (playerLife->getCurrentLife() < playerLife->getMaxLife())
			{
				playerLife->heal(healCount);
				getHost().destroy();
			}

		}
	}
}