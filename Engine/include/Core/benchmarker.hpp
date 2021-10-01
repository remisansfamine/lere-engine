#pragma once

#include <string>
#include <chrono>
#include <unordered_map>

#include "singleton.hpp"

namespace Core
{
	namespace Debug
	{
		class Benchmarker final : public Singleton<Benchmarker>
		{
			friend class Singleton<Benchmarker>;

		private:
			int reloadCount = 0;

			std::unordered_map<std::string, std::chrono::duration<double>> timeSums;

			class Timer
			{
			private:
				std::chrono::system_clock::time_point chronoStart;
				std::chrono::system_clock::time_point chronoEnd;

				bool hasEnded = false;

			public:
				Timer();

				void stop();

				template<typename T = std::chrono::system_clock>
				std::chrono::time_point<T> getStart() const
				{
					return chronoStart;
				}

				template<typename T = double>
				std::chrono::duration<T> getDuration()
				{
					if (!hasEnded)
						chronoEnd = std::chrono::system_clock::now();

					return chronoEnd - chronoStart;
				}
			};

			std::vector<std::unordered_map<std::string, Timer>> timers = { std::unordered_map<std::string, Timer>() };

		public:
			static void resetStatistics();

			static void startChrono(const std::string& chronoKey);
			static void stopChrono(const std::string& chronoKey);

			template<typename T = std::chrono::system_clock>
			static std::chrono::time_point<T> getTimerStart(const std::string& chronoKey)
			{
				return instance()->timers.back()[chronoKey].getStart<T>();
			}

			template<typename T = double>
			static std::chrono::duration<T> getDuration(const std::string& chronoKey)
			{
				return instance()->timers.back()[chronoKey].getDuration<T>();
			}

			static void drawImGui();

			static void sceneLoadedCallback();
		};
	}
}