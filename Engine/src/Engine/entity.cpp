#include "entity.hpp"

#include <sstream>
#include <imgui.h> 

#include "debug.hpp"
#include "render_manager.hpp"
#include "resources_manager.hpp"

#include "no_clip_movement.hpp"
#include "sprite_renderer.hpp"
#include "player_shooting.hpp"
#include "player_movement.hpp"
#include "camera_movement.hpp"
#include "sphere_collider.hpp"
#include "model_renderer.hpp"
#include "enemy_movement.hpp"
#include "ammo_counter.hpp"
#include "player_state.hpp"
#include "player_life.hpp"
#include "box_collider.hpp"
#include "pause_screen.hpp"
#include "lose_screen.hpp"
#include "win_screen.hpp"
#include "bullet_hole.hpp"
#include "enemy_state.hpp"
#include "game_master.hpp"
#include "enemy_life.hpp"
#include "main_menu.hpp"
#include "component.hpp"
#include "transform.hpp"
#include "rigidbody.hpp"
#include "life_bar.hpp"
#include "sky_box.hpp"
#include "button.hpp"
#include "medkit.hpp"
#include "camera.hpp"
#include "utils.hpp"
#include "light.hpp"

namespace Engine
{
	Entity::Entity(const std::string& name)
		: m_name(name)
	{
		Core::Debug::Log::info("Creating a Entity named " + name);
	}

	Entity::~Entity()
	{
	}

	void Entity::awakeComponents()
	{
		// Call the awake function for all the components
		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			component->awake();
		}
	}

	void Entity::updateComponents()
	{
		// Call the update function for all the components
		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			if (!component->isActive())
				continue;

			if (!component->hasStarted)
			{
				component->start();
				component->hasStarted = true;
			}

			component->update();
		}
	}

	void Entity::fixedUpdateComponents()
	{
		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			if (component->isActive())
				component->fixedUpdate();
		}
			
	}

	void Entity::lateUpdateComponents()
	{
		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			if (component->isActive())
				component->lateUpdate();
		}
	}

	void Entity::callCollisionEnter(const Physics::Collision& collision)
	{
		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			component->onCollisionEnter(collision);
		}
	}
	
	void Entity::callCollisionStay(const Physics::Collision& collision)
	{
		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			component->onCollisionStay(collision);
		}
	}

	void Entity::callCollisionExit(const Physics::Collision& collision)
	{
		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			component->onCollisionExit(collision);
		}
	}

	void Entity::callTriggerEnter(Physics::Collider* collider)
	{
		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			component->onTriggerEnter(collider);
		}
	}

	void Entity::callTriggerStay(Physics::Collider* collider)
	{
		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			component->onTriggerStay(collider);
		}
	}

	void Entity::callTriggerExit(Physics::Collider* collider)
	{
		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			component->onTriggerExit(collider);
		}
	}

	std::string Entity::toString()
	{
		std::string entityParse = "GO " + m_name + "\n";

		if (m_recipe != "")
			entityParse += "RECIPE " + m_recipe + '\n';

		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			entityParse += component->toString() + "\n";
		}

		return entityParse + "endGO\n\n";
	}

	void Entity::drawImGuiInspector()
	{
		ImGui::InputText(": Name", &m_name[0], 50);

		bool activated = isActive();

		if (ImGui::Checkbox("Enable", &activated))
			setActive(activated);

		if (ImGui::Button("Destroy"))
			destroy();

		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			component->drawImGui();
		}
	}

	void Entity::drawImGuiHierarchy(std::string& curEntityName, bool isDrawFromScene)
	{
		ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

		bool drawSelectable = true;

		Physics::TransformComponent* transform;
		if (tryGetComponent<Physics::TransformComponent>(transform))
		{
			bool canDraw = !(transform->hasParent() && isDrawFromScene);
			bool drawTree = transform->hasChild() && canDraw;
			drawSelectable = !transform->hasChild() && canDraw;

			if (drawTree)
			{
				int i = 0;
				static int selection_mask = (1 << 2);
				int node_clicked = -1;

				ImGuiTreeNodeFlags node_flags = base_flags;
				const bool is_selected = (selection_mask & (1 << i)) != 0;
				if (is_selected)
					node_flags |= ImGuiTreeNodeFlags_Selected;

				bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, m_name.c_str(), i);

				if (ImGui::IsItemClicked())
					curEntityName = m_name;

				if (m_name.compare(curEntityName) == 0)
				{
					ImGui::SameLine();
					ImGui::Text("   Selected");
				}

				if (node_open)
				{
					for (int index = 0; index < transform->getChildrenCount(); index++)
					{
						transform->getEntityChild(index).drawImGuiHierarchy(curEntityName, false);
					}

					ImGui::TreePop();
				}
			}
		}

		if (drawSelectable)
			Utils::selectImGuiString(m_name, curEntityName);
	}

	void Entity::parseComponents(std::istringstream& entityStream, std::string& parentName)
	{
		std::string comp;
		entityStream >> comp;

		if (comp == "TRANSFORM")
			Physics::TransformComponent::parseComponent(*this, entityStream, parentName);
		else if (comp == "RIGIDBODY")
			Physics::Rigidbody::parseComponent(*this, entityStream);
		else if (comp == "BOXCOLLIDER")
			Physics::BoxCollider::parseComponent(*this, entityStream);
		else if (comp == "SPHERECOLLIDER")
			Physics::SphereCollider::parseComponent(*this, entityStream);
		else if (comp == "MODELRENDERER")
			LowRenderer::ModelRenderer::parseComponent(*this, entityStream);
		else if (comp == "CAMERA")
			LowRenderer::Camera::parseComponent(*this, entityStream);
		else if (comp == "LIGHT")
			LowRenderer::Light::parseComponent(*this, entityStream);
		else if (comp == "SKYBOX")
			LowRenderer::SkyBox::parseComponent(*this, entityStream);
		else if (comp == "SPRITERENDERER")
			LowRenderer::SpriteRenderer::parseComponent(*this, entityStream);
		else if (comp == "PLAYERMOVEMENT")
			Gameplay::PlayerMovement::parseComponent(*this, entityStream);
		else if (comp == "PLAYERSTATE")
			Gameplay::PlayerState::parseComponent(*this, entityStream);
		else if (comp == "PLAYERLIFE")
			Gameplay::PlayerLife::parseComponent(*this, entityStream);
		else if (comp == "PLAYERSHOOTING")
			Gameplay::PlayerShooting::parseComponent(*this, entityStream);
		else if (comp == "ENEMYMOVEMENT")
			Gameplay::EnemyMovement::parseComponent(*this, entityStream);
		else if (comp == "ENEMYSTATE")
			Gameplay::EnemyState::parseComponent(*this, entityStream);
		else if (comp == "ENEMYLIFE")
			Gameplay::EnemyLife::parseComponent(*this, entityStream);
		else if (comp == "LIFEBAR")
			Gameplay::LifeBar::parseComponent(*this, entityStream);
		else if (comp == "AMMOCOUNTER")
			Gameplay::AmmoCounter::parseComponent(*this, entityStream);
		else if (comp == "MAINMENU")
			Gameplay::MainMenu::parseComponent(*this, entityStream);
		else if (comp == "LOSESCREEN")
			Gameplay::LoseScreen::parseComponent(*this, entityStream);
		else if (comp == "WINSCREEN")
			Gameplay::WinScreen::parseComponent(*this, entityStream);
		else if (comp == "PAUSESCREEN")
			Gameplay::PauseScreen::parseComponent(*this, entityStream);
		else if (comp == "GAMEMASTER")
			Gameplay::GameMaster::parseComponent(*this, entityStream);
		else if (comp == "CAMERAMOVEMENT")
			Gameplay::CameraMovement::parseComponent(*this, entityStream);
		else if (comp == "BUTTON")
			UI::Button::parseComponent(*this, entityStream);
		else if (comp == "BULLETHOLE")
			Gameplay::BulletHole::parseComponent(*this, entityStream);
		else if (comp == "NOCLIPMOVEMENT")
			NoClipMovement::parseComponent(*this, entityStream);
		else if (comp == "MEDKIT")
			Gameplay::MedKit::parseComponent(*this, entityStream);
	}

	void Entity::parseRecipe(const std::string& filePath, std::string& parentName)
	{
		m_recipe = filePath;

		std::istringstream recipeStream(Resources::ResourcesManager::loadRecipe(filePath)->recipe);

		std::string line;

		while (std::getline(recipeStream, line))
		{
			std::istringstream iss(line);

			std::string type;
			iss >> type;

			if (type == "COMP")
				parseComponents(iss, parentName);
		}
	}

	void Entity::parse(std::istream& scnStream, std::string& parentName)
	{
		std::string line;

		while (std::getline(scnStream, line))
		{
			std::istringstream iss(line);
			std::string type;
			iss >> type;

			if (type == "COMP")
				parseComponents(iss, parentName);
			else if (type == "RECIPE")
			{
				std::string filePath;
				iss >> filePath;
				parseRecipe(filePath, parentName);
			}
			else if (type == "endGO")
				break;
		}
	}

	void Entity::destroy()
	{
		Physics::TransformComponent* transform;
		if (tryGetComponent<Physics::TransformComponent>(transform))
		{
			for (int i = 0; i < transform->getChildrenCount(); i++)
				transform->getEntityChild(i).destroy();
		}

		for (auto& componentPair : m_components)
		{
			Component* component = componentPair.second.get();

			component->destroy();
		}

		Core::Engine::Graph::addToDestroyQueue(this);
	}

	void Entity::onDestroy()
	{
		Core::Engine::Graph::deleteEntity(m_name);
	}
}
