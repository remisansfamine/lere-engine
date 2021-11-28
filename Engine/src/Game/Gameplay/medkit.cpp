#include "medkit.hpp"

#include <imgui.h>

#include "collider.hpp"
#include "player_life.hpp"

namespace Gameplay
{
	MedKit::MedKit(Engine::Entity& owner)
		: Component(owner)
	{

	}

	void MedKit::start()
	{
		transform = getHost().getComponent<Physics::TransformComponent>();
	}

	void MedKit::update()
	{
		
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

	void MedKit::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		MedKit* mk;
		if (!owner.tryGetComponent(mk))
			mk = owner.addComponent<MedKit>();

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