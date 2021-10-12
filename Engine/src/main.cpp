#include <string>
#include <iostream>
#include <crtdbg.h>

#include "application.hpp"

#include "debug.hpp"

const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 900;

#include <python.h>
#include "pyhelper.hpp"

int main()
{
	// Check for leak
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try
	{
		CPyInstance pyInstance;


		Core::Application::init(SCR_WIDTH, SCR_HEIGHT, "Engine");

		Core::Application::update();
	}
	catch (const char* msg)
	{
		Core::Debug::Log::assertion(msg);
	}
	catch (const std::string& msg)
	{
		Core::Debug::Log::assertion(msg);
	}
	catch (const std::exception& exception)
	{
		Core::Debug::Log::assertion(exception.what());
	}
	catch (...)
	{
		Core::Debug::Log::assertion("Exception not supported.");
	}

	Core::Application::kill();

	return 0;
}