#pragma once

#include <vector>
#include <thread>
#include <functional>

#include <chrono>

#include "singleton.hpp"

#include "concurrent_queue.hpp"

class ThreadPool : public Singleton<ThreadPool>
{
    friend Singleton<ThreadPool>;

private:
    std::atomic<int> workingThreadCount;

    std::atomic_flag initialized = ATOMIC_FLAG_INIT;
    std::atomic_flag terminate = ATOMIC_FLAG_INIT;
    std::atomic<std::chrono::system_clock::time_point> lastTime = std::chrono::system_clock::now();

    std::vector<std::thread> workers;
    ConcurrentQueue<std::function<void()>> tasks;

    ConcurrentQueue<std::exception_ptr> exceptions;

    ~ThreadPool();

    static void infiniteLoop();

public:
    static void init(unsigned int workerCount = std::thread::hardware_concurrency());

    static void stopAllThread();

    static void syncAndClean();

    template <class Fct, typename... Types>
    static void addTask(Fct&& func, Types&&... args)
    {
        instance()->tasks.tryPush(std::bind(func, args...));
    }

    static void addTasks(const std::initializer_list<std::function<void()>>& tasksToAdd);

    static std::chrono::system_clock::time_point getLastTime();

    static void rethrowExceptions();
};