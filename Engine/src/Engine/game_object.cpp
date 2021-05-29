#include "game_object.hpp"

#include <sstream>
#include <imgui.h> 

#include "debug.hpp"
#include "render_manager.hpp"
#include "resources_manager.hpp"

#include "sprite_renderer.hpp"
#include "player_shooting.hpp"
#include "player_movement.hpp"
#include "camera_movement.hpp"
#include "sphere_collider.hpp"
#include "model_renderer.hpp"
#include "enemy_movement.hpp"
#include "player_state.hpp"
#include "box_collider.hpp"
#include "enemy_state.hpp"
#include "game_master.hpp"
#include "enemy_life.hpp"
#include "main_menu.hpp"
#include "component.hpp"
#include "transform.hpp"
#include "rigidbody.hpp"
#include "sky_box.hpp"
#include "button.hpp"
#include "camera.hpp"
#include "utils.hpp"
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




	void GameObject::awakeComponents()
	{
		// Call the awake function for all the components
		for (std::shared_ptr<Component>& component : m_components)
			component->awake();
	}

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

	void GameObject::callTriggerEnter(Physics::Collider* collider)
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->onTriggerEnter(collider);
	}

	void GameObject::callTriggerStay(Physics::Collider* collider)
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->onTriggerStay(collider);
	}

	void GameObject::callTriggerExit(Physics::Collider* collider)
	{
		for (std::shared_ptr<Component>& component : m_components)
			component->onTriggerExit(collider);
	}

	std::string GameObject::toString()
	{
		std::string goParse = "GO " + m_name + "\n";

		for (auto& comp : m_components)
			goParse += comp->toString() + "\n";

		return goParse + "endGO\n\n";
	}

	void GameObject::drawImGuiInspector()
	{
		ImGui::InputText(": Name", &m_name[0], 50);

		if (ImGui::Button("Destroy"))
			destroy();

		for (auto& component : m_components)
			component->drawImGui();
	}

	void GameObject::drawImGuiHierarchy(std::string& curDrawGoName, bool isDrawFromScene)
	{
		ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

		bool drawSelectable = true;

		std::shared_ptr<Physics::Transform> transform;
		if (tryGetComponent<Physics::Transform>(transform))
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
					curDrawGoName = m_name;

				if (m_name.compare(curDrawGoName) == 0)
				{
					ImGui::SameLine();
					ImGui::Text("   Selected");
				}

				if (node_open)
				{
					for (int index = 0; index < transform->getChildrenCount(); index++)
					{
						transform->getGOChild(index).drawImGuiHierarchy(curDrawGoName, false);
					}

					ImGui::TreePop();
				}
			}
		}

		if (drawSelectable)
			Utils::selectImGuiString(m_name, curDrawGoName);
	}

	void GameObject::parseComponents(std::istringstream& goStream, std::string& parentName)
	{
		std::string comp;
		goStream >> comp;

		if (comp == "TRANSFORM")
			Physics::Transform::parseComponent(*this, goStream, parentName);
		else if (comp == "RIGIDBODY")
			Physics::Rigidbody::parseComponent(*this, goStream);
		else if (comp == "BOXCOLLIDER")
			Physics::BoxCollider::parseComponent(*this, goStream);
		else if (comp == "SPHERECOLLIDER")
			Physics::SphereCollider::parseComponent(*this, goStream);
		else if (comp == "MODELRENDERER")
			LowRenderer::ModelRenderer::parseComponent(*this, goStream);
		else if (comp == "CAMERA")
			LowRenderer::Camera::parseComponent(*this, goStream);
		else if (comp == "LIGHT")
			LowRenderer::Light::parseComponent(*this, goStream);
		else if (comp == "SKYBOX")
			LowRenderer::SkyBox::parseComponent(*this, goStream);
		else if (comp == "SPRITERENDERER")
			LowRenderer::SpriteRenderer::parseComponent(*this, goStream);
		else if (comp == "PLAYERMOVEMENT")
			Gameplay::PlayerMovement::parseComponent(*this, goStream);
		else if (comp == "PLAYERSTATE")
			Gameplay::PlayerState::parseComponent(*this, goStream);
		else if (comp == "PLAYERSHOOTING")
			Gameplay::PlayerShooting::parseComponent(*this, goStream);
		else if (comp == "ENEMYMOVEMENT")
			Gameplay::EnemyMovement::parseComponent(*this, goStream);
		else if (comp == "ENEMYSTATE")
			Gameplay::EnemyState::parseComponent(*this, goStream);
		else if (comp == "ENEMYLIFE")
			Gameplay::EnemyLife::parseComponent(*this, goStream);
		else if (comp == "MAINMENU")
			Gameplay::MainMenu::parseComponent(*this, goStream);
		else if (comp == "GAMEMASTER")
			Gameplay::GameMaster::parseComponent(*this, goStream);
		else if (comp == "CAMERAMOVEMENT")
			Gameplay::CameraMovement::parseComponent(*this, goStream);
		else if (comp == "BUTTON")
			UI::Button::parseComponent(*this, goStream);
	}

	void GameObject::parseRecipe(const std::string& filePath, std::string& parentName)
	{
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

	void GameObject::parse(std::istream& scnStream, std::string& parentName)
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

	void GameObject::destroy()
	{
		for (auto& comp : m_components)
			comp->destroy();

		Core::Engine::Graph::addToDestroyQueue(this);
	}

	void GameObject::onDestroy()
	{

	}
}