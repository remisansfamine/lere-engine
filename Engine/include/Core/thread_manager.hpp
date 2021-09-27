#pragma once

#include <string>
#include <unordered_map>

#include "singleton.hpp"

#include "thread_pool.hpp"
#include "concurrent_queue.hpp"

namespace Multithread
{
    class ThreadManager : public Singleton<ThreadManager>
    {
        friend Singleton<ThreadManager>;

    private:
        std::unordered_map<std::string, ThreadPool> pools;

        bool monoThread = false;

    public:
        static void init(const std::string& poolKey, unsigned int workerCount = std::thread::hardware_concurrency());

        static std::size_t getWorkingThreadCount(const std::string& poolKey);

        static std::size_t getWorkerCount(const std::string& poolKey);

        static bool isEmpty(const std::string& poolKey);

        static bool isMonoThreaded();

        static void stopAllThread(const std::string& poolKey);
        static void stopAllPool();

        static void sync(const std::string& poolKey);
        static void syncAndClean(const std::string& poolKey);

        static void syncAll();
        static void syncAndCleanAll();

        template <class Fct, typename... Types>
        static void manageTask(const std::string& poolKey, Fct&& func, Types&&... args)
        {
            ThreadManager* TM = instance();
            if (TM->monoThread)
                std::bind(func, args...)();
            else
                TM->pools[poolKey].addTask(func, args...);
        }

        static std::chrono::system_clock::time_point getLastTime(const std::string& poolKey);

        static void rethrowExceptions();

        static void drawImGui();
    };
}