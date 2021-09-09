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

        if (std::queue<T>::empty())
        {
            used.clear();
            return false;
        }

        value = std::queue<T>::front();
        std::queue<T>::pop();

        used.clear();

        return true;
	}

	bool tryPush(const T& value)
	{
        while (used.test_and_set());

        std::queue<T>::push(value);

        used.clear();

        return true;
	}
};