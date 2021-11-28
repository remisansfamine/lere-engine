#include "lose_screen.hpp"

#include <algorithm> 
#include <imgui.h>

#include "inputs_manager.hpp"
#include "application.hpp"
#include "graph.hpp"
#include "time.hpp"
#include "button.hpp"

namespace Gameplay
{
	LoseScreen::LoseScreen(Engine::Entity& owner)
		: Component(owner)
	{

	}

	void LoseScreen::start()
	{
		buttons[0] = Core::Engine::Graph::findEntityWithName("LoseMainMenuButton");
		UI::Button* mainMenuptr = buttons[0]->getComponent<UI::Button>();

		mainMenuptr->addListener(UI::ButtonState::DOWN, []() {
			Core::TimeManager::setTimeScale(1.f);
			Core::Engine::Graph::setLoadScene("resources/scenes/mainMenu.scn");
			});

		mainMenuptr->addListener(UI::ButtonState::HIGHLIGHT, [mainMenuptr]() {
			mainMenuptr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});


		buttons[1] = Core::Engine::Graph::findEntityWithName("LoseExitButton");
		UI::Button* exitPtr = buttons[1]->getComponent<UI::Button>();
		exitPtr->addListener(UI::ButtonState::DOWN, []() {
			Core::Application::closeApplication();
			});
		exitPtr->addListener(UI::ButtonState::HIGHLIGHT, [exitPtr]() {
			exitPtr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
			});

		buttons[2] = Core::Engine::Graph::findEntityWithName("LoseText");

		for (int i = 0; i < 3; ++i)
			buttons[i]->setActive(false);
	}

	void LoseScreen::showLoseScreen(bool isActive)
	{
		Core::Engine::Graph::setCursorState(isActive);

		for (int i = 0; i < 3; ++i)
			buttons[i]->setActive(isActive);
	}

	void LoseScreen::drawImGui()
	{
		if (ImGui::TreeNode("LoseScreen"))
			ImGui::TreePop();
	}

	std::string LoseScreen::toString() const
	{
		return "COMP LOSESCREEN";
	}

	void LoseScreen::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		if (!owner.tryGetComponent<LoseScreen>())
			owner.addComponent<LoseScreen>();
	}
}