#include "inputs_manager.hpp"

#include "debug.hpp"

namespace Core::Input
{
	void InputManager::init(GLFWwindow* window)
	{
		InputManager* IM = instance();

		Input::window = window;

		addAxis("Horizontal", GLFW_KEY_A, GLFW_KEY_D);
		addAxis("Vertical", GLFW_KEY_S, GLFW_KEY_W);
		addAxis("UpDown", GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE);
		addButton("LockCursor", GLFW_KEY_C);
	}

	void InputManager::compute()
	{
		InputManager* IM = instance();

		for (auto buttonIt = IM->keyButtons.begin(); buttonIt != IM->keyButtons.end(); buttonIt++)
			buttonIt->second.compute();

		for (auto axisIt = IM->keyAxes.begin(); axisIt != IM->keyAxes.end(); axisIt++)
			axisIt->second.compute();
	}

	KeyButton& InputManager::getButtonByName(const std::string& name)
	{
		InputManager* IM = instance();

		auto keyIt = IM->keyButtons.find(name);

		Core::Debug::Assertion::out(keyIt != IM->keyButtons.end(), "Button " + name + " does not exist");

		return keyIt->second;
	}

	KeyAxis& InputManager::getAxisByName(const std::string& name)
	{
		InputManager* IM = instance();

		auto keyIt = IM->keyAxes.find(name);

		Core::Debug::Assertion::out(keyIt != IM->keyAxes.end(), "Axis " + name + " does not exist");

		return keyIt->second;
	}

	bool InputManager::getButtonDown(const std::string& name)
	{
		return getButtonByName(name).isButtonPressed();
	}

	bool InputManager::getButton(const std::string& name)
	{
		return getButtonByName(name).isButtonDown();
	}

	bool InputManager::getButtonUp(const std::string& name)
	{
		return getButtonByName(name).isButtonReleased();
	}

	float InputManager::getAxis(const std::string& name)
	{
		return getAxisByName(name).getValue();
	}

	void InputManager::addButton(const std::string& name, int keyID)
	{
		instance()->keyButtons.insert(std::make_pair(name, KeyButton(keyID)));
	}

	void InputManager::addAxis(const std::string& name, int negativeKeyID, int positiveKeyID)
	{
		instance()->keyAxes.insert(std::make_pair(name, KeyAxis(negativeKeyID, positiveKeyID)));
	}

	void InputManager::editButton(const std::string& name, int newKeyID)
	{
		getButtonByName(name).m_keyID = newKeyID;
	}

	void InputManager::editAxis(const std::string& name, int newPositiveKeyID, int newNegativeKeyID)
	{
		KeyAxis& axis = getAxisByName(name);

		axis.m_positiveKeyID = newPositiveKeyID;
		axis.m_negativeKeyID = newNegativeKeyID;
	}
}