#include "inputs_manager.hpp"

#include "debug.hpp"

namespace Core::Input
{
	void InputManager::init(GLFWwindow* _window)
	{
		InputManager* IM = instance();

		IM->window = _window;

		// Add default axes
		addAxis("Horizontal", GLFW_KEY_A, GLFW_KEY_D);
		addAxis("Vertical", GLFW_KEY_S, GLFW_KEY_W);
		addAxis("Forward", GLFW_KEY_W, GLFW_KEY_S);
		addAxis("MoveObjectHorizontal", GLFW_KEY_KP_4, GLFW_KEY_KP_6);
		addAxis("MoveObjectVertical", GLFW_KEY_KP_7, GLFW_KEY_KP_9);
		addAxis("MoveObjectForward", GLFW_KEY_KP_5, GLFW_KEY_KP_8);
		addAxis("UpDown", GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE);

		// Add default buttons
		addButton("LockCursor", GLFW_KEY_C);
		addButton("Jump", GLFW_KEY_SPACE);

		// Add default mouse buttons
		addMouseButton("RightClick", GLFW_MOUSE_BUTTON_RIGHT);
		addMouseButton("LeftClick", GLFW_MOUSE_BUTTON_LEFT);
		addMouseButton("MiddleClick", GLFW_MOUSE_BUTTON_MIDDLE);
	}

	void InputManager::compute()
	{
		InputManager* IM = instance();

		// Compute buttons
		for (auto buttonIt = IM->keyButtons.begin(); buttonIt != IM->keyButtons.end(); buttonIt++)
			buttonIt->second.compute(IM->window);

		// Compute axes
		for (auto axisIt = IM->keyAxes.begin(); axisIt != IM->keyAxes.end(); axisIt++)
			axisIt->second.compute(IM->window);

		// Compute mouse buttons
		for (auto mouseIt = IM->mouseButtons.begin(); mouseIt != IM->mouseButtons.end(); mouseIt++)
			mouseIt->second.compute(IM->window);

		// Compute mouse motion
		{
			double newMouseX, newMouseY;

			glfwGetCursorPos(IM->window, &newMouseX, &newMouseY);

			// Calculate mouse position differencies with the last frame
			IM->deltasMouse.x = (float)(newMouseX - IM->mousePosition.x);
			IM->deltasMouse.y = (float)(newMouseY - IM->mousePosition.y);
			IM->mousePosition = Core::Maths::vec2((float)newMouseX, (float)newMouseY);
		}
	}

	KeyButton& InputManager::getButtonByName(const std::string& name)
	{
		InputManager* IM = instance();

		auto keyIt = IM->keyButtons.find(name);

		// Assert if the key does not exist
		Core::Debug::Assertion::out(keyIt != IM->keyButtons.end(), "Button " + name + " does not exist");

		return keyIt->second;
	}

	MouseButton& InputManager::getMouseButtonByName(const std::string& name)
	{
		InputManager* IM = instance();

		auto keyIt = IM->mouseButtons.find(name);

		// Assert if the key does not exist
		Core::Debug::Assertion::out(keyIt != IM->mouseButtons.end(), "Mouse button " + name + " does not exist");

		return keyIt->second;
	}

	KeyAxis& InputManager::getAxisByName(const std::string& name)
	{
		InputManager* IM = instance();

		auto keyIt = IM->keyAxes.find(name);

		// Assert if the axis does not exist
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

	bool InputManager::getMouseButtonDown(const std::string& name)
	{
		return getMouseButtonByName(name).isButtonPressed();
	}

	bool InputManager::getMouseButton(const std::string& name)
	{
		return getMouseButtonByName(name).isButtonDown();
	}

	bool InputManager::getMouseButtonUp(const std::string& name)
	{
		return getMouseButtonByName(name).isButtonReleased();
	}

	float InputManager::getAxis(const std::string& name)
	{
		return getAxisByName(name).getValue();
	}

	void InputManager::addButton(const std::string& name, int keyID)
	{
		instance()->keyButtons.insert(std::make_pair(name, KeyButton(keyID)));
	}

	void InputManager::addMouseButton(const std::string& name, int keyID)
	{
		instance()->mouseButtons.insert(std::make_pair(name, MouseButton(keyID)));
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

	Core::Maths::vec2 InputManager::getDeltasMouse()
	{
		return instance()->deltasMouse;
	}

	Core::Maths::vec2 InputManager::getMousePos()
	{
		return instance()->mousePosition;
	}
}