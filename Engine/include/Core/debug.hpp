#pragma once

#include "singleton.hpp"
#include "concurrent_queue.hpp"

#include <chrono>
#include <ctime>

#include <iostream>
#include <array>

#include <thread>

namespace Core
{
	namespace Debug
	{
		enum class LogType
		{
			ERROR,
			ASSERTION,
			WARNING,
			INFO,
			EXCEPTION,
			GLOBAL
		};

		struct LogInfo
		{
			time_t time = std::time(0);
			std::string message;
			LogType type;

			LogInfo() = default;
			LogInfo(const std::string& message, LogType type, const time_t& time = std::time(0));

			std::string getMessage(const std::string& timeFormat);
		};

		class Log : public Singleton<Log>
		{
			friend class Singleton<Log>;

		private:
			std::thread printThread;

			ConcurrentQueue<LogInfo> queue;
			std::string logsAsString[(int)LogType::GLOBAL + 1];
			
			std::atomic_flag terminate;

			Log();
			~Log();

			static void saveToFile(const std::string& fileLocation, LogType typeToSave);
			static void saveToFiles();

			// Output the casted log
			static void out();

		public:
			static void exception(const std::string& log);

			static void warning(const std::string& log);

			static void assertion(const std::string& log);

			static void error(const std::string& log);

			static void info(const std::string& log);
		};

		class Assertion : public Singleton<Assertion>
		{
			friend class Singleton<Assertion>;

		private:
			Assertion();
			~Assertion();

		public:
			// Get assertion
			static void out(const std::exception& assertion);

			// Get any supported type and check is the condition return true
			template <typename T>
			static void out(bool condition, const T& assertion)
			{
				// If the condition does not return true, return an error
				if (!condition)
					throw std::string(assertion);
			}
		};
	};
}