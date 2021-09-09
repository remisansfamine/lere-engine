#pragma once

#include <vector>
#include <thread>
#include <functional>

#include "singleton.hpp"

#include "concurrent_queue.hpp"

class ThreadPool : public Singleton<ThreadPool>
{
    friend Singleton<ThreadPool>;

private:
    std::atomic_flag initialized = ATOMIC_FLAG_INIT;
    std::atomic_flag terminate = ATOMIC_FLAG_INIT;
    std::atomic<long> lastTime = (const long)std::time(0);

    std::vector<std::thread> workers;
    ConcurrentQueue<std::function<void()>> tasks;

    ~ThreadPool();

    static void infiniteLoop();

public:
    static void init(int workerCount = std::thread::hardware_concurrency());

    static void stopAllThread();

    template <typename T>
    static void addTask(const T& taskToAdd)
    {
        instance()->tasks.tryPush(taskToAdd);
    }

    static void addTasks(const std::initializer_list<std::function<void()>>& tasksToAdd);

    static const std::time_t& getLastTime();
};