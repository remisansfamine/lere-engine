#pragma once

#include <vector>
#include <string>
#include <memory>

#include "object.hpp"

#include "debug.hpp"

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

		template <class C, class ...Crest, typename B = std::enable_if_t<std::is_base_of<Component, C>::value>>
		void constexpr addComponent(Crest... args)
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

			Core::Debug::Assertion::out(tryGetComponent(componentToReturn), "Component not found");

			return componentToReturn;
		}

		//void callColisions(Collider);
		void awakeComponents();
		void startComponents();
		void updateComponents();

		void destroy() override {}
	};
}