#include "thread_pool.hpp"

void ThreadPool::infiniteLoop()
{
    ThreadPool* TP = instance();

    while (!TP->terminate.test())
    {
        std::function<void()> task;

        if (!TP->tasks.tryPop(task))
            continue;

        task();
        TP->lastTime = std::time(0);
    }
}

void ThreadPool::init(int workerCount)
{
    ThreadPool* TP = instance();

    if (TP->initialized.test_and_set() && !TP->terminate.test())
        return;

    for (int i = 0; i < workerCount; i++)
        TP->workers.emplace_back(ThreadPool::infiniteLoop);
}

void ThreadPool::stopAllThread()
{
    ThreadPool* TP = instance();

    if (TP->terminate.test_and_set())
        return;

    for (auto& worker : TP->workers)
        worker.join();
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

const std::time_t& ThreadPool::getLastTime()
{
    return instance()->lastTime;
}
