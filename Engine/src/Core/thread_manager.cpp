#include "thread_manager.hpp"
#include "debug.hpp"

#include "imgui.h"

namespace Multithread
{
    ThreadManager::~ThreadManager()
    {
        stopAllPool();
    }

    void ThreadManager::init(const std::string& poolKey, unsigned int workerCount)
    {
        // Init the pool at the correct key
        instance()->pools[poolKey].init(workerCount);
    }

    std::size_t ThreadManager::getWorkingThreadCount(const std::string& poolKey)
    {
        // Return the working thread number from the pool at the correct key
        return instance()->pools[poolKey].getWorkingThreadCount();
    }

    std::size_t ThreadManager::getWorkerCount(const std::string& poolKey)
    {
        return instance()->pools[poolKey].getWorkerCount();
    }

    bool ThreadManager::isEmpty(const std::string& poolKey)
    {
        // Return if the pool at the correct key is empty
        return instance()->pools[poolKey].isEmpty();
    }

    bool ThreadManager::isMonoThreaded()
    {
        return instance()->monoThread;
    }

    void ThreadManager::stopAllThread(const std::string& poolKey)
    {
        instance()->pools[poolKey].stopAllThread();
    }

    void ThreadManager::stopAllPool()
    {
        ThreadManager* TM = instance();

        for (auto& poolPair : TM->pools)
            poolPair.second.stopAllThread();
    }

    void ThreadManager::sync(const std::string& poolKey)
    {
        instance()->pools[poolKey].sync();
    }

    void ThreadManager::syncAndClean(const std::string& poolKey)
    {
        instance()->pools[poolKey].syncAndClean();
    }

    void ThreadManager::syncAll()
    {
        ThreadManager* TM = instance();

        for (auto& poolPair : TM->pools)
            poolPair.second.sync();
    }

    void ThreadManager::syncAndCleanAll()
    {
        ThreadManager* TM = instance();

        for (auto& poolPair : TM->pools)
            poolPair.second.syncAndClean();
    }

    std::chrono::system_clock::time_point ThreadManager::getLastTime(const std::string& poolKey)
    {
        return instance()->pools[poolKey].getLastTime();
    }

    void ThreadManager::rethrowExceptions()
    {
        ThreadManager* TM = instance();

        for (auto& poolPair : TM->pools)
            poolPair.second.rethrowExceptions();
    }

    void ThreadManager::drawImGui()
    {
        ThreadManager* TM = instance();

        if (ImGui::Begin("Thread Manager"))
        {
            ImGui::Checkbox("Is load mono-threaded", &TM->monoThread);

            for (const auto& poolPair : TM->pools)
            {
                if (ImGui::TreeNode(poolPair.first.c_str()))
                {
                    std::string workingThreadString = "Working threads = " + std::to_string(poolPair.second.getWorkingThreadCount());
                    ImGui::Text(workingThreadString.c_str());

                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }
}