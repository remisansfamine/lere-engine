#pragma once

#include <string>
#include <memory>
#include <istream>
#include <unordered_map>

#include "object.hpp"
#include "collision.hpp"

#include "debug.hpp"

namespace Physics
{
	class Collider;
}

namespace Engine
{
	class Component;

	class Entity : public Object
	{
	private:

	public:
		bool isStatic = false;

		std::string m_name = "Entity";

		std::string m_recipe = "";

		std::unordered_map<Component*, std::unique_ptr<Component>> m_components;

		Entity() = default;
		Entity(const std::string& name);
		virtual ~Entity();

		template <class C, typename ...Args, typename Base = std::enable_if_t<std::is_base_of_v<Component, C> && !std::is_same_v<Component, C>>>
		C* addComponent(Args... args)
		{
			auto compPtr = std::make_unique<C>(*this, args...);
			C* rawPtr = compPtr.get();

			m_components[rawPtr] = std::move(compPtr);

			return rawPtr;
		}

		template <class C, typename Base = std::enable_if_t<std::is_base_of_v<Component, C> && !std::is_same_v<Component, C>>>
		bool tryGetComponent()
		{
			C* componentToReturn;
			return tryGetComponent(componentToReturn);
		}

		template <class C, typename Base = std::enable_if_t<std::is_base_of_v<Component, C> && !std::is_same_v<Component, C>>>
		bool tryGetComponent(C*& componentToReturn)
		{
			for (auto& componentPair : m_components)
			{
				Component* rawPtr = componentPair.second.get();

				auto castedComponent = dynamic_cast<C*>(rawPtr);

				if (!castedComponent)
					continue;

				componentToReturn = castedComponent;
				return true;
			}

			return false;
		}

		template <class C, typename Base = std::enable_if_t<std::is_base_of<Component, C>::value>>
		C* getComponent()
		{
			C* componentToReturn;
			
			bool hasComponent = tryGetComponent(componentToReturn);

			if (!hasComponent)
				Core::Debug::Log::error("Component not found");

			return componentToReturn;
		}

		void awakeComponents();
		void updateComponents();
		void fixedUpdateComponents();
		void lateUpdateComponents();

		void callCollisionEnter(const Physics::Collision& collision);
		void callCollisionStay(const Physics::Collision& collision);
		void callCollisionExit(const Physics::Collision& collision);

		void callTriggerEnter(Physics::Collider* collider);
		void callTriggerStay(Physics::Collider* collider);
		void callTriggerExit(Physics::Collider* collider);

		void drawImGuiInspector();
		void drawImGuiHierarchy(std::string& curEntityName, bool isDrawFromScene);

		std::string toString();

		void parseComponents(std::istringstream& parseComponent, std::string& parentName);
		void parseRecipe(const std::string& filePath, std::string& parentName);
		void parse(std::istream& scnStream, std::string& parentName);

		void destroy() override;
		void onDestroy() override;
	};

}