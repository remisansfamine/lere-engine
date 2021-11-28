#pragma once

namespace Core
{
	class Manager
	{
	private:
		bool initialized = false;

	protected:
		void setInitializationState();

	public:
		bool hasBeenInitialized();
	};
}