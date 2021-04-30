#include "key_button.hpp"

namespace Core::Input
{
	KeyButton::KeyButton(int keyID)
		: m_keyID(keyID)
	{

	}

	void KeyButton::compute()
	{
		m_wasDown = m_isDown;
		m_isDown = glfwGetKey(window, m_keyID);

		m_isPressed = !m_wasDown && m_isDown;
		m_isReleased = m_wasDown && !m_isDown;
	}

	bool KeyButton::isButtonPressed()
	{
		return m_isPressed;
	}

	bool KeyButton::isButtonReleased()
	{
		return m_isReleased;
	}

	bool KeyButton::isButtonDown()
	{
		return m_isDown;
	}
}