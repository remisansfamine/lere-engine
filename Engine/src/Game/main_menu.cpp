#include "main_menu.hpp"

#include <algorithm> 
#include <imgui.h>

#include "inputs_manager.hpp"
#include "application.hpp"
#include "graph.hpp"

#include "button.hpp"

namespace Gameplay
{
	MainMenu::MainMenu(Engine::Entity& owner)
		: Component(owner)
	{

	}

	void MainMenu::start()
	{
		Core::Engine::Graph::setCursorState(true);

		Engine::Entity* goButtonNewGame = Core::Engine::Graph::findEntityWithName("NewGameButton");
		UI::Button* newGameptr = goButtonNewGame->getComponent<UI::Button>();
		newGameptr->addListener(UI::ButtonState::DOWN, [](){
			Core::Engine::Graph::setLoadScene("resources/scenes/defaultScene.scn");
		});
		newGameptr->addListener(UI::ButtonState::HIGHLIGHT, [newGameptr]() {
			newGameptr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
		});

		Engine::Entity* goButtonExit = Core::Engine::Graph::findEntityWithName("ExitButton");
		UI::Button* exitPtr = goButtonExit->getComponent<UI::Button>();
		exitPtr->addListener(UI::ButtonState::DOWN, [](){
			Core::Application::closeApplication();
		});
		exitPtr->addListener(UI::ButtonState::HIGHLIGHT, [exitPtr]() {
			exitPtr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
		});
	}

	void MainMenu::update()
	{
	}

	void MainMenu::drawImGui()
	{
		if (ImGui::TreeNode("MainMenu"))
		{
			
			ImGui::TreePop();
		}
	}

	std::string MainMenu::toString() const
	{
		return "COMP MAINMENU";
	}

	void MainMenu::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		if (!owner.tryGetComponent<MainMenu>())
			owner.addComponent<MainMenu>();
	}
}