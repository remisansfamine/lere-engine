#include "benchmarker.hpp"

#include <imgui.h>

#include "utils.hpp"
#include "time.hpp"

#include "graph.hpp"

namespace Core::Debug
{
	Benchmarker::Timer::Timer()
		: chronoStart(std::chrono::system_clock::now())
	{
		
	}

	void Benchmarker::Timer::stop()
	{
		hasEnded = true;

		chronoEnd = std::chrono::system_clock::now();
	}

	void Benchmarker::resetStatistics()
	{
		Benchmarker* BM = instance();

		BM->timeSums.clear();
		BM->timers = { std::unordered_map<std::string, Timer>() };
	}

	void Benchmarker::startChrono(const std::string& chronoKey)
	{
		instance()->timers.back()[chronoKey] = Timer();
	}

	void Benchmarker::stopChrono(const std::string& chronoKey)
	{
		instance()->timers.back()[chronoKey].stop();
	}

	void Benchmarker::drawImGui()
	{
		Benchmarker* BM = instance();

		if (ImGui::Begin("Benchmarker"))
		{
			ImGui::InputInt("Reload count", &BM->reloadCount);

			ImGui::Text("DeltaTime %f", Core::TimeManager::getDeltaTime());

			if (ImGui::Button("Reload"))
				Core::Engine::Graph::reloadScene(true);

			if (ImGui::Button("Reset statistics"))
				resetStatistics();

			if (ImGui::CollapsingHeader("Averages"))
			{
				for (const auto& sum : BM->timeSums)
				{
					double average = sum.second.count() / BM->timers.size();
					std::string averageString = sum.first + " " + std::to_string(average);
					ImGui::Text(averageString.c_str());
				}
			}

			if (ImGui::CollapsingHeader("Timers"))
			{
				for (int i = 0; i < BM->timers.size(); i++)
				{
					auto& timersMap = BM->timers[i];

					std::string mapIndex = "Benchmark number " + std::to_string(i);

					if (ImGui::TreeNode(mapIndex.c_str()))
					{
						for (auto& timerPair : timersMap)
						{
							if (ImGui::TreeNode(timerPair.first.c_str()))
							{
								time_t timerStart = std::chrono::system_clock::to_time_t(timerPair.second.getStart());

								std::string startString = "Start = " + Utils::getTimeAsString("%X", timerStart);
								ImGui::Text(startString.c_str());

								std::string durationString = "Duration = " + std::to_string(timerPair.second.getDuration().count() * 1000) + " ms.";
								ImGui::Text(durationString.c_str());

								ImGui::TreePop();
							}
						}

						ImGui::TreePop();
					}
				}
			}

		}
		ImGui::End();
	}

	void Benchmarker::sceneLoadedCallback()
	{
		Benchmarker* BM = instance();

		if (!BM->reloadCount)
			return;

		if (--BM->reloadCount > 0)
		{
			for (auto& timerPair : BM->timers.back())
				BM->timeSums[timerPair.first] += timerPair.second.getDuration();

			BM->timers.push_back(std::unordered_map<std::string, Timer>());
			Core::Engine::Graph::reloadScene(true);
		}
	}
}