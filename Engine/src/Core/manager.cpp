#include "manager.hpp"

namespace Core
{
	void Manager::setInitializationState()
	{
		initialized = true;
	}

	bool Manager::hasBeenInitialized()
	{
		return initialized;
	}
}