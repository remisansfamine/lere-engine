#include "game_object.hpp"

#include "component.hpp"

#include "debug.hpp"
#include "sprite_renderer.hpp"

#include "render_manager.hpp"

#include "transform.hpp"

namespace Engine
{
	GameObject::GameObject(const std::string& name)
		: m_name(name)
	{
		Core::Debug::Log::info("Creating a GameObject named " + name);

		addComponent<LowRenderer::SpriteRenderer>();
		addComponent<LowRenderer::Light>();
	}

	GameObject::~GameObject()
	{
	}

	// TODO: CALL AWAKE AT THE CORRECT TIME
	void GameObject::awakeComponents()
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->awake();
	}

	// TODO: CALL START AT THE CORRECT TIME
	void GameObject::startComponents()
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->start();
	}

	void GameObject::updateComponents()
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->update();
	}
}