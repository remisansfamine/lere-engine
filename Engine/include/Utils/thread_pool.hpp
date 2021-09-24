#pragma once

#include <vector>
#include <thread>
#include <functional>

#include <chrono>

#include "singleton.hpp"

#include "concurrent_queue.hpp"

class ThreadPool
{
private:
    std::atomic<int> workingThreadCount;

    std::atomic<bool> terminate = false;
    std::atomic<bool> initialized = false;
    std::atomic<std::chrono::system_clock::time_point> lastTime = std::chrono::system_clock::now();

    std::vector<std::thread> workers;
    ConcurrentQueue<std::function<void()>> tasks;

    ConcurrentQueue<std::exception_ptr> exceptions;


    void infiniteLoop();

public:
    ~ThreadPool();

    void init(unsigned int workerCount = std::thread::hardware_concurrency());

    void stopAllThread();

    void sync();
    void syncAndClean();

    template <class Fct, typename... Types>
    void addTask(Fct&& func, Types&&... args)
    {
        tasks.tryPush(std::bind(func, args...));
    }

    std::chrono::system_clock::time_point getLastTime();

    void rethrowExceptions();
};