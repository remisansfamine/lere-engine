#include "enemy_life.hpp"

namespace Gameplay
{
	EnemyLife::EnemyLife(Engine::GameObject& gameObject)
		: EntityLife(gameObject, std::shared_ptr<EnemyLife>(this))
	{

	}

	void EnemyLife::drawImGui()
	{
		if (ImGui::TreeNode("EnemyLife"))
		{
			ImGui::DragInt("Life : ", &life);
			ImGui::TreePop();
		}
	}

	std::string EnemyLife::toString() const
	{
		return "COMP LIFE " + std::to_string(life);
	}

	void EnemyLife::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		gameObject.addComponent<EnemyLife>();
		auto enemy = gameObject.getComponent<EnemyLife>();

		iss >> enemy->life;
	}

	void EnemyLife::hurt(int damage)
	{
		EntityLife::hurt(damage);
	}

	void EnemyLife::kill()
	{
		EntityLife::kill();
	}
}