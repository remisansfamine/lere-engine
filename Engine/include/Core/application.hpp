#pragma once

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "singleton.hpp"
#include "maths.hpp"

namespace Core
{
	class Application final : public Singleton<Application> 
	{
		friend class Singleton<Application>;

	private:
		bool initialized = false;
		GLFWwindow* window = nullptr;

		Application();
		~Application();

		[[nodiscard]] static GLFWwindow* createWindow(unsigned int screenWidth, unsigned int screenHeight, const char* title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);

	public:
		static void init(unsigned int screenWidth, unsigned int screenHeight, const char* title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
		static void update();
		static float getAspect();
		static void setCursor(bool isCursorLock);
		static void getDeltasMouse(Core::Maths::vec2& deltaMouse);
	};
}