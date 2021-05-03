#pragma once

#include <unordered_map>
#include <string>

#include "singleton.hpp"

#include "key_button.hpp"
#include "key_axis.hpp"
#include "maths.hpp"

namespace Core::Input
{
	class InputManager final : public Singleton<InputManager>
	{
		friend Singleton<InputManager>;

	private:
		bool initialized = false;
		GLFWwindow* window = nullptr;

		Core::Maths::vec2 mousePosition;
		Core::Maths::vec2 deltasMouse;

		std::unordered_map<std::string, KeyAxis> keyAxes;
		std::unordered_map<std::string, KeyButton> keyButtons;

		static KeyButton& getButtonByName(const std::string& name);
		static KeyAxis& getAxisByName(const std::string& name);

	public:

		static void init(GLFWwindow* window);

		static void compute();

		static bool getButtonDown(const std::string& name);
		static bool getButton(const std::string& name);
		static bool getButtonUp(const std::string& name);

		static float getAxis(const std::string& name);

		static void addButton(const std::string& name, int keyID);
		static void addAxis(const std::string& name, int positiveKeyID, int negativeKeyID);

		static void editButton(const std::string& name, int newKeyID);
		static void editAxis(const std::string& name, int newPositiveKeyID, int newNegativeKeyID);

		static Core::Maths::vec2 getDeltasMouse();
	};
}