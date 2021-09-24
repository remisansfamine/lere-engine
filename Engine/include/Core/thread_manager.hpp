#pragma once

#include <vector>
#include <thread>
#include <functional>

#include <chrono>

#include "singleton.hpp"

#include "thread_pool.hpp"

#include "concurrent_queue.hpp"

class ThreadManager : public Singleton<ThreadManager>
{
    friend Singleton<ThreadManager>;

private:
    ~ThreadManager();

    ThreadPool mainPool;
    
    bool monoThread = false;

public:
    static void init(unsigned int workerCount = std::thread::hardware_concurrency());

    static void stopAllThread();

    static void sync();
    static void syncAndClean();


    template <class Fct, typename... Types>
    static void manageTask(Fct&& func, Types&&... args)
    {
        ThreadManager* TM = instance();
        if (TM->monoThread)
            std::bind(func, args...)();
        else
            TM->mainPool.addTask(func, args...);
    }

    static std::chrono::system_clock::time_point getLastTime();

    static void rethrowExceptions();

    static void drawImGui();
};