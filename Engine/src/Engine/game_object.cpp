#include "game_object.hpp"

#include <sstream>
#include <imgui.h> 

#include "debug.hpp"
#include "render_manager.hpp"

#include "sprite_renderer.hpp"
#include "player_movement.hpp"
#include "sphere_collider.hpp"
#include "model_renderer.hpp"
#include "player_state.hpp"
#include "box_collider.hpp"
#include "component.hpp"
#include "transform.hpp"
#include "rigidbody.hpp"
#include "sky_box.hpp"
#include "camera.hpp"
#include "light.hpp"

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
		{
			if (!component->hasStarted)
			{
				component->start();
				component->hasStarted = true;
			}

			component->update();
		}
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




	void GameObject::callCollisionEnter(const Physics::Collision& collision)
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->onCollisionEnter(collision);
	}
	
	void GameObject::callCollisionStay(const Physics::Collision& collision)
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->onCollisionStay(collision);
	}

	void GameObject::callCollisionExit(const Physics::Collision& collision)
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->onCollisionExit(collision);
	}




	std::string GameObject::toString()
	{
		std::string goParse = "GO " + m_name + "\n";

		for (auto& comp : m_components)
			goParse += comp->toString() + "\n";

		return goParse + "endGO\n\n";
	}

	void GameObject::drawImGui()
	{
		ImGui::InputText(": Name", &m_name[0], 50);

		for (auto& component : m_components)
			component->drawImGui();
	}

	void GameObject::parse(std::istream& scnStream, std::string& parentName)
	{
		std::string line;
		std::string type;

		while (std::getline(scnStream, line))
		{
			std::istringstream iss(line);
			iss >> type;

			if (type == "COMP")
			{
				std::string comp;
				iss >> comp;

				if (comp == "TRANSFORM")
				{
					Physics::Transform::parseComponent(*this, iss, parentName);
				}
				else if (comp == "RIGIDBODY")
					Physics::Rigidbody::parseComponent(*this, iss);
				else if (comp == "BOXCOLLIDER")
					Physics::BoxCollider::parseComponent(*this, iss);
				else if (comp == "SPHERECOLLIDER")
					Physics::SphereCollider::parseComponent(*this, iss);
				else if (comp == "MODELRENDERER")
					LowRenderer::ModelRenderer::parseComponent(*this, iss);
				else if (comp == "CAMERA")
					LowRenderer::Camera::parseComponent(*this, iss);
				else if (comp == "LIGHT")
					LowRenderer::Light::parseComponent(*this, iss);
				else if (comp == "SKYBOX")
					LowRenderer::SkyBox::parseComponent(*this, iss);
				else if (comp == "SPRITERENDERER")
					LowRenderer::SpriteRenderer::parseComponent(*this, iss);
				else if (comp == "PLAYERMOVEMENT")
					Gameplay::PlayerMovement::parseComponent(*this, iss);
				else if (comp == "PLAYERSTATE")
					Gameplay::PlayerState::parseComponent(*this, iss);
			}
			else if (type == "endGO")
				break;
		}
	}
}