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

		class Log final : public Singleton<Log>
		{
			friend class Singleton<Log>;

		private:
			std::thread printThread;

			ConcurrentQueue<LogInfo> queues[(int)LogType::GLOBAL + 1];
			std::string logsAsString[(int)LogType::GLOBAL + 1];
			
			std::atomic_flag terminate;

			Log();
			~Log();

			static void saveToFile(const std::string& fileLocation, LogType typeToSave);
			static void saveToFiles();

			// Output the casted log
			static void out();

		public:
			template <typename T>
			static void exception(const T& log)
			{
				LogType type = LogType::EXCEPTION;
				instance()->queues[(int)type].tryPush(LogInfo(std::string("EXCEPTION: ") + std::string(log), type));
				instance()->queues[(int)LogType::GLOBAL].tryPush(LogInfo(std::string("EXCEPTION: ") + std::string(log), LogType::GLOBAL));
			}

			template <typename T>
			static void warning(const T& log)
			{
				LogType type = LogType::WARNING;
				instance()->queues[(int)type].tryPush(LogInfo(std::string("Warning: ") + std::string(log), type));
				instance()->queues[(int)LogType::GLOBAL].tryPush(LogInfo(std::string("Warning: ") + std::string(log), LogType::GLOBAL));
			}

			template <typename T>
			static void assertion(const T& log)
			{
				LogType type = LogType::ASSERTION;
				instance()->queues[(int)type].tryPush(LogInfo(std::string("ASSERTION: ") + std::string(log), type));
				instance()->queues[(int)LogType::GLOBAL].tryPush(LogInfo(std::string("ASSERTION: ") + std::string(log), LogType::GLOBAL));
			}

			template <typename T>
			static void error(const T& log)
			{
				LogType type = LogType::ERROR;
				instance()->queues[(int)type].tryPush(LogInfo(std::string("ERROR: ") + std::string(log), type));
				instance()->queues[(int)LogType::GLOBAL].tryPush(LogInfo(std::string("ERROR: ") + std::string(log), LogType::GLOBAL));
			}

			template <typename T>
			static void info(const T& log)
			{
				LogType type = LogType::INFO;
				instance()->queues[(int)type].tryPush(LogInfo(std::string("Info: ") + std::string(log), type));
				instance()->queues[(int)LogType::GLOBAL].tryPush(LogInfo(std::string("Info: ") + std::string(log), LogType::GLOBAL));
			}
		};

		class Assertion final : public Singleton<Assertion>
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