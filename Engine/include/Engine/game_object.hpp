#pragma once

#include <vector>
#include <string>
#include <memory>

#include "object.hpp"

#include "debug.hpp"

namespace Physics
{
	class Collider;
}

namespace Engine
{
	class Component;

	class GameObject : public Object
	{
	private:


	public:
		bool isStatic = false;

		std::string m_name = "GameObject";

		std::vector<std::shared_ptr<Component>> m_components;

		GameObject(const std::string& name);
		virtual ~GameObject();

		template <class C, class ...Args, typename B = std::enable_if_t<std::is_base_of<Component, C>::value>>
		void constexpr addComponent(Args... args)
		{
			new C(*this, args...);
		}

		template <class C, typename B = std::enable_if_t<std::is_base_of<Component, C>::value>>
		bool tryGetComponent()
		{
			std::shared_ptr<C> componentToReturn;
			return tryGetComponent(componentToReturn);
		}

		template <class C, typename B = std::enable_if_t<std::is_base_of<Component, C>::value>>
		bool tryGetComponent(std::shared_ptr<C>& componentToReturn)
		{
			for (std::shared_ptr<Component>& component : m_components)
			{
				auto castedComponent = std::dynamic_pointer_cast<C>(component);

				if (!castedComponent)
					continue;

				componentToReturn = castedComponent;
				return true;
			}

			return false;
		}

		template <class C, typename B = std::enable_if_t<std::is_base_of<Component, C>::value>>
		[[nodiscard]] std::shared_ptr<C> getComponent()
		{
			std::shared_ptr<C> componentToReturn;
			
			bool hasComponent = tryGetComponent(componentToReturn);

			if (!hasComponent)
				Core::Debug::Log::error("Component not found");

			return componentToReturn;
		}

		//void callColisions(Collider);
		void awakeComponents();
		void startComponents();
		void updateComponents();
		void fixedUpdateComponents();
		void lateUpdateComponents();

		void callCollisionEnter(std::shared_ptr<Physics::Collider> other);
		void callCollisionStay(std::shared_ptr<Physics::Collider> other);
		void callCollisionExit(std::shared_ptr<Physics::Collider> other);

		void drawImGui();

		std::string toString();

		void destroy() override {}
	};
}