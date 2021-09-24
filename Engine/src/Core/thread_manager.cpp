#include "thread_manager.hpp"
#include "debug.hpp"

#include "imgui.h"

void ThreadManager::init(unsigned int workerCount)
{
    instance()->mainPool.init(workerCount);
}

void ThreadManager::stopAllThread()
{
    instance()->mainPool.stopAllThread();
}

void ThreadManager::sync()
{
    instance()->mainPool.sync();
}

void ThreadManager::syncAndClean()
{
    instance()->mainPool.syncAndClean();
}

ThreadManager::~ThreadManager()
{
    stopAllThread();
}

std::chrono::system_clock::time_point ThreadManager::getLastTime()
{
    return instance()->mainPool.getLastTime();
}

void ThreadManager::rethrowExceptions()
{
    instance()->mainPool.rethrowExceptions();
}

void ThreadManager::drawImGui()
{
	ThreadManager* TM = instance();

	if (ImGui::Begin("Thread Manager"))
	{
		ImGui::Checkbox("Is load mono-threaded", &TM->monoThread);

	}
	ImGui::End();
}