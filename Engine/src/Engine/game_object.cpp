#include "game_object.hpp"

#include "imgui.h"

#include "model_renderer.hpp"
#include "render_manager.hpp"
#include "debug.hpp"

#include "component.hpp"
#include "transform.hpp"

namespace Engine
{
	GameObject::GameObject(const std::string& name)
		: m_name(name)
	{
		Core::Debug::Log::info("Creating a GameObject named " + name);
	}

	GameObject::~GameObject()
	{
	}




	// TODO: CALL AWAKE AT THE CORRECT TIME
	void GameObject::awakeComponents()
	{
		// Call the awake function for all the components
		for (std::shared_ptr<Component>& component : m_components)
			component->awake();
	}

	// TODO: CALL START AT THE CORRECT TIME
	void GameObject::startComponents()
	{
		// Call the start function for all the components
		for (std::shared_ptr<Component>& component : m_components)
			component->start();
	}

	void GameObject::updateComponents()
	{
		// Call the update function for all the components
		for (std::shared_ptr<Component>& component : m_components)
			component->update();
	}

	void GameObject::fixedUpdateComponents()
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->fixedUpdate();
	}

	void GameObject::lateUpdateComponents()
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->lateUpdate();
	}

	void GameObject::callCollisionEnter(std::shared_ptr<Physics::Collider> other)
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->onCollisionEnter(other);
	}
	
	void GameObject::callCollisionStay(std::shared_ptr<Physics::Collider> other)
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->onCollisionStay(other);
	}

	void GameObject::callCollisionExit(std::shared_ptr<Physics::Collider> other)
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->onCollisionExit(other);
	}

	void GameObject::drawImGui()
	{
		ImGui::InputText(": Name", &m_name[0], 50);

		for (auto& component : m_components)
			component->drawImGui();
	}
}