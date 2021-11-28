#include "object.hpp"

namespace Engine
{
	bool Object::isActive() const
	{
		return isEnable;
	}

	void Object::setActive(bool value)
	{
		isEnable = value;
	}
}