#include "debug.hpp"

#include <fstream>
#include <cassert>

#include "utils.hpp"

namespace Core
{
	namespace Debug
	{
		LogInfo::LogInfo(const std::string& message, LogType type, const time_t& time)
			: message(message), type(type), time(time)
		{

		}

		std::string LogInfo::getMessage(const std::string& timeFormat)
		{
			return Utils::getTimeAsString("[%x - %X]", time) + ' ' + message + '\n';
		}

		Log::Log()
		{
			printThread = std::thread(&Log::out);
		}

		Log::~Log()
		{
			Log::info("Destroying the Logs Manager");

			Log::saveToFiles();

			if (terminate.test_and_set())
				return;

			printThread.join();
		}

		void Log::saveToFile(const std::string& fileLocation, LogType typeToSave)
		{
			Log* logManager = instance();

			// Create a new log file
			std::ofstream currentFile(fileLocation);

			std::string& logString = logManager->logsAsString[(int)typeToSave];

			// Put all the logs in it
			currentFile << logString;

			// Clear the logs
			logString.clear();
		}

		void Log::saveToFiles()
		{
			saveToFile("logs/infos.txt", LogType::INFO);
			saveToFile("logs/log.txt", LogType::GLOBAL);
			saveToFile("logs/assertions.txt", LogType::ASSERTION);
			saveToFile("logs/warnings.txt", LogType::WARNING);
			saveToFile("logs/errors.txt", LogType::ERROR);
			saveToFile("logs/exception.txt", LogType::EXCEPTION);
		}

		void Log::out()
		{
			Log* logManager = instance();

			while (!logManager->terminate.test())
			{
				LogInfo log;
				if (!logManager->queue.tryPop(log))
					continue;

				// Get the time as a string
				std::string message = log.getMessage("[%x - %X]");

				// Put the logs in the Log Manager 
				logManager->logsAsString[(int)log.type] += message;
				logManager->logsAsString[(int)LogType::GLOBAL] += message;

				// Cout the current log
				std::cout << message;
			}
		}

		void Log::exception(const std::string& log)
		{
			instance()->queue.tryPush(LogInfo(std::string("EXCEPTION: ") + std::string(log), LogType::EXCEPTION));
		}

		void Log::warning(const std::string& log)
		{
			instance()->queue.tryPush(LogInfo(std::string("Warning: ") + std::string(log), LogType::WARNING));
		}

		void Log::assertion(const std::string& log)
		{
			instance()->queue.tryPush(LogInfo(std::string("ASSERTION: ") + std::string(log), LogType::ASSERTION));
		}

		void Log::error(const std::string& log)
		{
			instance()->queue.tryPush(LogInfo(std::string("ERROR: ") + std::string(log), LogType::ERROR));
		}

		void Log::info(const std::string& log)
		{
			instance()->queue.tryPush(LogInfo(std::string("Info: ") + std::string(log), LogType::INFO));
		}

		Assertion::Assertion()
		{
			Log::info("Creating the Assertions Manager");
		}

		Assertion::~Assertion()
		{
			Log::info("Destroying the Assertions Manager");
		}

		void Assertion::out(const std::exception& assertion)
		{
			// Throw assertion if an exception has been handled
			throw  assertion;
		}
	}
}