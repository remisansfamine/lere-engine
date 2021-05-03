#include "component.hpp"

#include "transform.hpp"

namespace Engine
{
	Component::Component(GameObject& gameObject, const std::shared_ptr<Component>& childPtr)
		: m_gameObject(gameObject)
	{
		m_gameObject.m_components.push_back(childPtr);
	}

	Component::~Component()
	{
	}

	void Component::setActive(bool value)
	{
		// Call the correct function when state changed
		if (isActive() != value)
			value ? onEnable() : onDisable();

		// Activate or deactivate the component
		Object::setActive(value);
	}

	void Component::destroy()
	{
	}

	GameObject& Component::getHost()
	{
		return m_gameObject;
	}
}