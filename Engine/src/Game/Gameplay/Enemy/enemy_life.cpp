#include "enemy_life.hpp"

namespace Gameplay
{
	EnemyLife::EnemyLife(Engine::Entity& owner)
		: EntityLife(owner, "resources/sounds/enemyDamage.ogg", "resources/sounds/enemyDeath.wav")
	{
	}

	void EnemyLife::start()
	{
		gameMaster = Core::Engine::Graph::findEntityWithName("GameMaster")->getComponent<GameMaster>();
		gameMaster->enemyCount++;
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
		return "COMP ENEMYLIFE " + EntityLife::toString();
	}

	void EnemyLife::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		EnemyLife* el;
		if (!owner.tryGetComponent(el))
			el = owner.addComponent<EnemyLife>();

		iss >> el->life;
		iss >> el->maxLife;
	}

	void EnemyLife::hurt(int damage)
	{
		EntityLife::hurt(damage);
	}

	void EnemyLife::kill()
	{
		gameMaster->removeEnemy();

		EntityLife::kill();

		getHost().destroy();
	}
}