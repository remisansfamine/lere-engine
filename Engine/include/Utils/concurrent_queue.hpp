#pragma once

#include <queue>
#include <atomic>

template <typename T>
class ConcurrentQueue : public std::queue<T>
{
    std::atomic_flag used = ATOMIC_FLAG_INIT;

public:
	bool tryPop(T& value)
	{
        while (used.test_and_set());

        // Check if the queue is empty
        if (std::queue<T>::empty())
        {
            used.clear();
            return false;
        }

        // Set the value reference with the front of the queue, and remove it
        value = std::queue<T>::front();
        std::queue<T>::pop();

        used.clear();

        return true;
	}

	bool tryPush(const T& value)
	{
        while (used.test_and_set());

        // Push if the queue is not used
        std::queue<T>::push(value);

        used.clear();

        return true;
	}

    void clear()
    {
        while (used.test_and_set());

        // Swap the queue with an empty one
        std::queue<T> emptyQueue;
        std::swap(*this, emptyQueue);

        used.clear();
    }
};