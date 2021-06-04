#include "player_life.hpp"

#include "graph.hpp"
#include "collider.hpp"
#include "inputs_manager.hpp"

namespace Gameplay
{
	PlayerLife::PlayerLife(Engine::GameObject& gameObject)
		: EntityLife(gameObject, std::shared_ptr<PlayerLife>(this))
	{

	}

	void PlayerLife::start()
	{
		lifeBar = Core::Engine::Graph::findGameObjectWithName(lifeBarName)->getComponent<LifeBar>();
	}

	void PlayerLife::update()
	{
		if (life <= 0)
			Core::Debug::Log::info("You lose!");

		if (Core::Input::InputManager::getMouseButtonDown("RightClick"))
			hurt();
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

	void PlayerLife::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::shared_ptr<PlayerLife> pl;
		if (!gameObject.tryGetComponent(pl))
			pl = gameObject.addComponent<PlayerLife>();

		iss >> pl->life;
		iss >> pl->maxLife;
		iss >> pl->lifeBarName;
	}
}