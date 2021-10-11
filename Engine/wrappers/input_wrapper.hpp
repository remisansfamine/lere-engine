#pragma once

#include <string>

namespace Scripting
{
	namespace Input
	{
		bool getButtonDown(const std::string& name);
		bool getButton(const std::string& name);
		bool getButtonUp(const std::string& name);

		bool getMouseButtonDown(const std::string& name);
		bool getMouseButton(const std::string& name);
		bool getMouseButtonUp(const std::string& name);

		float getAxis(const std::string& name);

		void addButton(const std::string& name, int keyID);
		void addMouseButton(const std::string& name, int keyID);
		void addAxis(const std::string& name, int positiveKeyID, int negativeKeyID);

		void editButton(const std::string& name, int newKeyID);
		void editAxis(const std::string& name, int newPositiveKeyID, int newNegativeKeyID);
	}
}