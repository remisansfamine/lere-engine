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
		Core::Engine::Graph::setCursorState(true);
	}
	
	void PauseScreen::update()
	{
		Engine::GameObject* goButtonResumeGame = Core::Engine::Graph::findGameObjectWithName("ResumeGameButton");
		std::shared_ptr<UI::Button> buttonResumeGame = goButtonResumeGame->getComponent<UI::Button>();
		UI::Button* resumeGameptr = buttonResumeGame.get();
		resumeGameptr->addListener(UI::ButtonState::DOWN, []() {
			Core::TimeManager::setTimeScale(1.f);
			});
		resumeGameptr->addListener(UI::ButtonState::HIGHLIGHT, [resumeGameptr]() {
			resumeGameptr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});


		Engine::GameObject* goButtonMainScreen = Core::Engine::Graph::findGameObjectWithName("MainScreenButton");
		std::shared_ptr<UI::Button> buttonMainScreen = goButtonMainScreen->getComponent<UI::Button>();
		UI::Button* mainScreenptr = buttonMainScreen.get();
		mainScreenptr->addListener(UI::ButtonState::DOWN, []() {
			Core::Engine::Graph::loadMainMenu();
			});
		mainScreenptr->addListener(UI::ButtonState::HIGHLIGHT, [mainScreenptr]() {
			mainScreenptr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});


		Engine::GameObject* goButtonSaveScene = Core::Engine::Graph::findGameObjectWithName("SaveSceneButton");
		std::shared_ptr<UI::Button> buttonSaveScene = goButtonSaveScene->getComponent<UI::Button>();
		UI::Button* saveSceneptr = buttonSaveScene.get();
		saveSceneptr->addListener(UI::ButtonState::DOWN, []() {
			Core::Engine::Graph::saveCurrentScene();
			Core::Debug::Log::info("Scene has been saved");
			});
		saveSceneptr->addListener(UI::ButtonState::HIGHLIGHT, [saveSceneptr]() {
			saveSceneptr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
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