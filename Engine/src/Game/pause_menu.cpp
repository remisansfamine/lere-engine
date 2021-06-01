#include "pause_menu.hpp"

#include <algorithm> 
#include <imgui.h>

#include "inputs_manager.hpp"
#include "application.hpp"
#include "graph.hpp"

#include "button.hpp"

namespace Gameplay
{
	PauseMenu::PauseMenu(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<PauseMenu>(this))
	{

	}

	void PauseMenu::start()
	{
		Core::Engine::Graph::setCursorState(true);

		Engine::GameObject* goButtonResumeGame = Core::Engine::Graph::findGameObjectWithName("ResumeGameButton");
		std::shared_ptr<UI::Button> buttonResumeGame = goButtonResumeGame->getComponent<UI::Button>();
		UI::Button* newGameptr = buttonResumeGame.get();
		resumeGameptr->addListener(UI::ButtonState::DOWN, []() {
			Core::Engine::Graph::loadResumeGame(); // or loadSaveGame ?
			});
		resumeGameptr->addListener(UI::ButtonState::HIGHLIGHT, [resumeGameptr]() {
			resumeGameptr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});


		Engine::GameObject* goButtonMainMenu = Core::Engine::Graph::findGameObjectWithName("MainMenuButton");
		std::shared_ptr<UI::Button> buttonMainMenu = goButtonMainMenu->getComponent<UI::Button>();
		UI::Button* newGameptr = buttonMainMenu.get();
		mainMenuptr->addListener(UI::ButtonState::DOWN, []() {
			Core::Engine::Graph::loadMainMenu();
			});
		mainMenuptr->addListener(UI::ButtonState::HIGHLIGHT, [mainMenuptr]() {
			mainMenuptr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});

		
		Engine::GameObject* goButtonOptions = Core::Engine::Graph::findGameObjectWithName("OptionsButton");
		std::shared_ptr<UI::Button> buttonOptions = goButtonOptions->getComponent<UI::Button>();
		UI::Button* optionsPtr = buttonOptions.get();
		optionsPtr->addListener(UI::ButtonState::DOWN, []() {
			Core::Debug::Log::info("Options");
			});
		optionsPtr->addListener(UI::ButtonState::HIGHLIGHT, [optionsPtr]() {
			optionsPtr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});


		Engine::GameObject* goButtonExit = Core::Engine::Graph::findGameObjectWithName("ExitButton");
		std::shared_ptr<UI::Button> buttonExit = goButtonExit->getComponent<UI::Button>();
		UI::Button* exitPtr = buttonExit.get();
		exitPtr->addListener(UI::ButtonState::DOWN, []() {
			Core::Application::closeApplication();
			});
		exitPtr->addListener(UI::ButtonState::HIGHLIGHT, [exitPtr]() {
			exitPtr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});
	}

	void PauseMenu::drawImGui()
	{
		if (ImGui::TreeNode("PauseMenu"))
			ImGui::TreePop();
	}

	std::string PauseMenu::toString() const
	{
		return "COMP PAUSEMENU";
	}

	void PauseMenu::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<PauseMenu>())
			gameObject.addComponent<PauseMenu>();
	}
}