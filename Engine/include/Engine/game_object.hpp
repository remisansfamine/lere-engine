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
		std::string m_name;
		std::vector<std::shared_ptr<Component>> m_components;

		GameObject(const std::string& name);
		~GameObject();

		template <typename C>
		void constexpr addComponent()
		{
			new C(*this);
		}

		template <typename C>
		bool tryGetComponent(std::shared_ptr<C>& componentToReturn)
		{
			for (std::shared_ptr<Component> component : m_components)
			{
				auto castedComponent = std::dynamic_pointer_cast<C>(component);

				if (castedComponent != nullptr)
				{
					componentToReturn = castedComponent;
					return true;
				}
			}

			return false;
		}

		template <typename C>
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