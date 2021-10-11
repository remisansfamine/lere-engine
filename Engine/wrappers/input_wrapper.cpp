#include "input_wrapper.hpp"

#include "inputs_manager.hpp"

namespace Scripting
{
	namespace Input
	{
		bool getButtonDown(const std::string& name)
		{
			return Core::Input::InputManager::getButtonDown(name);
		}

		bool getButton(const std::string& name)
		{
			return Core::Input::InputManager::getButton(name);
		}

		bool getButtonUp(const std::string& name)
		{
			return Core::Input::InputManager::getButtonUp(name);
		}

		bool getMouseButtonDown(const std::string& name)
		{
			return Core::Input::InputManager::getMouseButtonDown(name);
		}

		bool getMouseButton(const std::string& name)
		{
			return Core::Input::InputManager::getMouseButton(name);
		}

		bool getMouseButtonUp(const std::string& name)
		{
			return Core::Input::InputManager::getMouseButtonUp(name);
		}

		float getAxis(const std::string& name)
		{
			return Core::Input::InputManager::getAxis(name);
		}

		void addButton(const std::string& name, int keyID)
		{
			Core::Input::InputManager::addButton(name, keyID);
		}

		void addMouseButton(const std::string& name, int keyID)
		{
			Core::Input::InputManager::addMouseButton(name, keyID);
		}

		void addAxis(const std::string& name, int positiveKeyID, int negativeKeyID)
		{
			Core::Input::InputManager::addAxis(name, positiveKeyID, negativeKeyID);
		}

		void editButton(const std::string& name, int newKeyID)
		{
			Core::Input::InputManager::editButton(name, newKeyID);
		}

		void editAxis(const std::string& name, int newPositiveKeyID, int newNegativeKeyID)
		{
			Core::Input::InputManager::editAxis(name, newPositiveKeyID, newNegativeKeyID);
		}
	}
}