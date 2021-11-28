#pragma once

#include <vector>
#include <thread>
#include <functional>

#include <chrono>

#include "singleton.hpp"

#include "concurrent_queue.hpp"

namespace Multithread
{
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

        std::size_t threadsCount = 0u;

    public:
        ~ThreadPool();

        void init(unsigned int workerCount);

        std::size_t getWorkingThreadCount() const;

        std::size_t getWorkerCount() const;

        bool isEmpty() const;

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
}