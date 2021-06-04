#pragma once

template <class T>
class Singleton
{
protected:
	Singleton() = default;

	static T* currentInstance;

	[[nodiscard]] static T* instance()
	{
		if (!currentInstance)
			currentInstance = new T();

		return currentInstance;
	}

public:
	static void kill()
	{
		delete currentInstance;
		currentInstance = nullptr;
	}

	Singleton(T&) = delete;

	void operator=(const T&) = delete;
};

template <class T> T* Singleton<T>::currentInstance = nullptr;