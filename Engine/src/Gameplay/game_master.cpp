#include "game_master.hpp"

#include "imgui.h"

#include "inputs_manager.hpp"
#include "graph.hpp"
#include "time.hpp"

namespace Gameplay
{
	GameMaster::GameMaster(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<GameMaster>(this))
	{

	}

	void GameMaster::update()
	{
		if (Core::Input::InputManager::getButtonDown("Return"))
			Core::Engine::Graph::loadMainMenu();
		if (Core::Input::InputManager::getButtonDown("Save"))
		{
			Core::Engine::Graph::saveCurrentScene();
			Core::Debug::Log::info("Scene has been saved");
		}
	}

	void GameMaster::drawImGui()
	{
		if (ImGui::TreeNode("GameMaster"))
		{
			ImGui::Text("F5 to save the scene");
			ImGui::Text("ESCAPE tu return main menu");
			ImGui::TreePop();
		}
	}

	std::string GameMaster::toString() const
	{
		return "COMP GAMEMASTER";
	}

	void GameMaster::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<GameMaster>())
			gameObject.addComponent<GameMaster>();
	}
}