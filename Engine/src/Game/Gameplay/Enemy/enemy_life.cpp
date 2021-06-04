#include "enemy_life.hpp"


namespace Gameplay
{
	EnemyLife::EnemyLife(Engine::GameObject& gameObject)
		: EntityLife(gameObject, std::shared_ptr<EnemyLife>(this), "resources/sounds/enemyDamage.ogg", "resources/sounds/enemyDeath.wav")
	{
	}

	void EnemyLife::start()
	{
		transform = getHost().getComponent<Physics::Transform>();
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

	void EnemyLife::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::shared_ptr<EnemyLife> el;
		if (!gameObject.tryGetComponent(el))
			el = gameObject.addComponent<EnemyLife>();

		iss >> el->life;
		iss >> el->maxLife;
	}

	void EnemyLife::hurt(int damage)
	{
		EntityLife::hurt(damage);
	}

	void EnemyLife::kill()
	{
		EntityLife::kill();

		if (true)
		{
			auto& go = Core::Engine::Graph::instantiate("MediKit", "resources/recipes/medikit.recipe");
			go.getComponent<Physics::Transform>()->m_position = transform->m_position;
		}

		getHost().destroy();
	}
}
