#include "player_life.hpp"

#include "graph.hpp"
#include "collider.hpp"
#include "inputs_manager.hpp"

namespace Gameplay
{
	PlayerLife::PlayerLife(Engine::Entity& owner)
		: EntityLife(owner)
	{

	}

	void PlayerLife::start()
	{
		gameMaster = Core::Engine::Graph::findEntityWithName("GameMaster")->getComponent<GameMaster>();
		lifeBar = Core::Engine::Graph::findEntityWithName(lifeBarName)->getComponent<LifeBar>();
	}

	void PlayerLife::drawImGui()
	{
		if (ImGui::TreeNode("PlayerLife"))
		{
			EntityLife::drawImGui();

			ImGui::TreePop();
		}
	}

	std::string PlayerLife::toString() const
	{
		return "COMP PLAYERLIFE " + EntityLife::toString() + " " + lifeBarName;
	}

	void PlayerLife::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		PlayerLife* pl;
		if (!owner.tryGetComponent(pl))
			pl = owner.addComponent<PlayerLife>();

		iss >> pl->life;
		iss >> pl->maxLife;
		iss >> pl->lifeBarName;
	}

	void PlayerLife::kill()
	{
		gameMaster->removePlayer();

		EntityLife::kill();

		getHost().destroy();
	}
}