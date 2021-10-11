#include "debug_wrapper.hpp"

#include "debug.hpp"

namespace Scripting
{
	namespace Debug
	{
		void exception(const std::string& infoLog)
		{
			Core::Debug::Log::exception(infoLog);
		}

		void assertion(const std::string& infoLog)
		{
			Core::Debug::Log::assertion(infoLog);
		}

		void warning(const std::string& infoLog)
		{
			Core::Debug::Log::warning(infoLog);
		}

		void error(const std::string& infoLog)
		{
			Core::Debug::Log::error(infoLog);
		}

		void info(const std::string& infoLog)
		{
			Core::Debug::Log::info(infoLog);
		}
	}
}