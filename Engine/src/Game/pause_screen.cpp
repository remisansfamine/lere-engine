#include "pause_screen.hpp"

#include <algorithm> 
#include <imgui.h>

#include "inputs_manager.hpp"
#include "application.hpp"
#include "graph.hpp"
#include "time.hpp"
#include "button.hpp"

namespace Gameplay
{
	PauseScreen::PauseScreen(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<PauseScreen>(this))
	{

	}

	void PauseScreen::start()
	{
		gameMaster = Core::Engine::Graph::findGameObjectWithName("GameMaster")->getComponent<GameMaster>();

		buttons[0] = Core::Engine::Graph::findGameObjectWithName("ResumeGameButton");
		std::shared_ptr<UI::Button> buttonResumeGame = buttons[0]->getComponent<UI::Button>();
		UI::Button* resumeGameptr = buttonResumeGame.get();

		resumeGameptr->addListener(UI::ButtonState::DOWN, [this]() {
			Core::TimeManager::setTimeScale(1.f);
			showPauseScreen(false);
			gameMaster->isPaused = false;
			});

		resumeGameptr->addListener(UI::ButtonState::HIGHLIGHT, [resumeGameptr]() {
			resumeGameptr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});


		buttons[1] = Core::Engine::Graph::findGameObjectWithName("MainMenuButton");
		std::shared_ptr<UI::Button> buttonMainMenu = buttons[1]->getComponent<UI::Button>();
		UI::Button* mainMenuptr = buttonMainMenu.get();

		mainMenuptr->addListener(UI::ButtonState::DOWN, []() {
			Core::TimeManager::setTimeScale(1.f);
			Core::Engine::Graph::loadMainMenu();
			});

		mainMenuptr->addListener(UI::ButtonState::HIGHLIGHT, [mainMenuptr]() {
			mainMenuptr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});


		/*buttons[2] = Core::Engine::Graph::findGameObjectWithName("SaveSceneButton");
		std::shared_ptr<UI::Button> buttonSaveScene = buttons[2]->getComponent<UI::Button>();
		UI::Button* saveSceneptr = buttonSaveScene.get();

		saveSceneptr->addListener(UI::ButtonState::DOWN, []() {
			Core::Engine::Graph::saveCurrentScene();
			Core::Debug::Log::info("Scene has been saved");
			});

		saveSceneptr->addListener(UI::ButtonState::HIGHLIGHT, [saveSceneptr]() {
			saveSceneptr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});*/


		buttons[2] = Core::Engine::Graph::findGameObjectWithName("ExitButton");
		std::shared_ptr<UI::Button> buttonExit = buttons[2]->getComponent<UI::Button>();
		UI::Button* exitPtr = buttonExit.get();
		exitPtr->addListener(UI::ButtonState::DOWN, []() {
			Core::Application::closeApplication();
			});
		exitPtr->addListener(UI::ButtonState::HIGHLIGHT, [exitPtr]() {
			exitPtr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});

		for (int i = 0; i < 3; ++i)
			buttons[i]->setActive(false);
	}

	void PauseScreen::showPauseScreen(bool isActive)
	{
		Core::Engine::Graph::setCursorState(isActive);

		for (int i = 0; i < 3; ++i)
			buttons[i]->setActive(isActive);
	}

	void PauseScreen::drawImGui()
	{
		if (ImGui::TreeNode("PauseScreen"))
			ImGui::TreePop();
	}

	std::string PauseScreen::toString() const
	{
		return "COMP PAUSESCREEN";
	}

	void PauseScreen::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<PauseScreen>())
			gameObject.addComponent<PauseScreen>();
	}
}