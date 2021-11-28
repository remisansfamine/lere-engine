#include "component.hpp"

#include <algorithm>
#include <memory>

#include <imgui.h>

#include "graph.hpp"

#include "transform.hpp"
#include "collider.hpp"

namespace Engine
{
	Component::Component(Entity& owner)
		: owner(owner)
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
		bool activated = Object::isActive();

		if (ImGui::Checkbox("Enable", &activated))
			setActive(activated);

		if (ImGui::Button("Destroy"))
			destroy();
	}

	void Component::onDestroy()
	{
		if (owner.m_components.size() == 0)
			return;

		owner.m_components.erase(this);
	}

	bool Component::isActive() const
	{
		return getHost().isActive() && Object::isActive();
	}

	void Component::destroy()
	{
		Core::Engine::Graph::addToDestroyQueue(this);
	}

	Entity& Component::getHost()
	{
		return owner;
	}

	const Entity& Component::getHost() const
	{
		return owner;
	}
}