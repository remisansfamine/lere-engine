#include "thread_pool.hpp"
#include "debug.hpp"

void ThreadPool::infiniteLoop()
{
    ThreadPool* TP = instance();

    while (!TP->terminate)
    {
        std::function<void()> task;

        if (!TP->tasks.tryPop(task))
            continue;

        try
        {
            TP->workingThreadCount++;
            task();
            TP->workingThreadCount--;
        }
        catch (...)
        {
            TP->exceptions.tryPush(std::current_exception());
        }

        TP->lastTime = std::chrono::system_clock::now();
    }
}

void ThreadPool::init(unsigned int workerCount)
{
    ThreadPool* TP = instance();

    if (TP->initialized && !TP->terminate)
        return;

    TP->initialized = true;

    for (unsigned int i = 0; i < workerCount; i++)
        TP->workers.emplace_back(ThreadPool::infiniteLoop);
}

void ThreadPool::stopAllThread()
{
    ThreadPool* TP = instance();

    if (TP->terminate)
        return;

    TP->terminate = true;

    for (auto& worker : TP->workers)
        worker.join();
}

void ThreadPool::sync()
{
    ThreadPool* TP = instance();

    while (TP->workingThreadCount > 0);
}

void ThreadPool::syncAndClean()
{
    instance()->tasks.clear();
    sync();
}

ThreadPool::~ThreadPool()
{
    stopAllThread();
}

void ThreadPool::addTasks(const std::initializer_list<std::function<void()>>& tasksToAdd)
{
    ThreadPool* TP = instance();

    for (const auto& task : tasksToAdd)
        TP->tasks.tryPush(task);
}

std::chrono::system_clock::time_point ThreadPool::getLastTime()
{
    return instance()->lastTime;
}

void ThreadPool::rethrowExceptions()
{
    std::exception_ptr exception;

    if (instance()->exceptions.tryPop(exception))
        std::rethrow_exception(exception);
}