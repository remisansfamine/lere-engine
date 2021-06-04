#include "win_screen.hpp"

#include <algorithm> 
#include <imgui.h>

#include "inputs_manager.hpp"
#include "application.hpp"
#include "graph.hpp"
#include "time.hpp"
#include "button.hpp"

namespace Gameplay
{
	WinScreen::WinScreen(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<WinScreen>(this))
	{

	}

	void WinScreen::start()
	{
		buttons[0] = Core::Engine::Graph::findGameObjectWithName("WinMainMenuButton");
		std::shared_ptr<UI::Button> buttonMainMenu = buttons[0]->getComponent<UI::Button>();
		UI::Button* mainMenuptr = buttonMainMenu.get();

		mainMenuptr->addListener(UI::ButtonState::DOWN, []() {
			Core::TimeManager::setTimeScale(1.f);
			Core::Engine::Graph::loadMainMenu();
			});

		mainMenuptr->addListener(UI::ButtonState::HIGHLIGHT, [mainMenuptr]() {
			mainMenuptr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});


		buttons[1] = Core::Engine::Graph::findGameObjectWithName("WinExitButton");
		std::shared_ptr<UI::Button> buttonExit = buttons[1]->getComponent<UI::Button>();
		UI::Button* exitPtr = buttonExit.get();
		exitPtr->addListener(UI::ButtonState::DOWN, []() {
			Core::Application::closeApplication();
			});
		exitPtr->addListener(UI::ButtonState::HIGHLIGHT, [exitPtr]() {
			exitPtr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});

		buttons[2] = Core::Engine::Graph::findGameObjectWithName("WinText");


		for (int i = 0; i < 3; ++i)
			buttons[i]->setActive(false);
	}

	void WinScreen::showWinScreen(bool isActive)
	{
		Core::Engine::Graph::setCursorState(isActive);

		for (int i = 0; i < 3; ++i)
			buttons[i]->setActive(isActive);
	}

	void WinScreen::drawImGui()
	{
		if (ImGui::TreeNode("WinScreen"))
			ImGui::TreePop();
	}

	std::string WinScreen::toString() const
	{
		return "COMP WINSCREEN";
	}

	void WinScreen::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<WinScreen>())
			gameObject.addComponent<WinScreen>();
	}
}