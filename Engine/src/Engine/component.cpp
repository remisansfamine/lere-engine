#include "component.hpp"

#include <algorithm>
#include <imgui.h>

#include "graph.hpp"

#include "transform.hpp"
#include "collider.hpp"

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

	void Component::drawImGui()
	{
		if (ImGui::Button("Destroy test"))
			destroy();
	}

	void Component::onDestroy()
	{
		for (auto it = m_gameObject.m_components.begin(); it != m_gameObject.m_components.end(); it++)
		{
			if (it->get() == this)
			{
				m_gameObject.m_components.erase(it);
				break;
			}
		}
	}

	void Component::destroy()
	{
		Core::Engine::Graph::addToDestroyQueue(this);
	}

	GameObject& Component::getHost()
	{
		return m_gameObject;
	}
}