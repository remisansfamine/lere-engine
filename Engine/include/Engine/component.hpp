#pragma once

#include <memory>

#include "object.hpp"

#include "game_object.hpp"

namespace Engine
{
	class Component : public Object
	{
	private:
		GameObject& m_gameObject;

		//friend class GameObject;

	protected:
		Component(GameObject& gameObject, const std::shared_ptr<Component>& childPtr);
		virtual ~Component();

		template <class C, class ...Crest, typename B = std::enable_if_t<std::is_base_of<Component, C>::value>>
		std::shared_ptr<C> requireComponent(Crest... args)
		{
			std::shared_ptr<C> tempPtr;

			if (!m_gameObject.tryGetComponent<C>(tempPtr))
			{
				m_gameObject.addComponent<C>(args...);
				return m_gameObject.getComponent<C>();
			}

			return tempPtr;
		}

	public:

		void setActive(bool value) override;
		void destroy() override;

		void virtual draw() { }
		void virtual awake() { }
		void virtual start() { }
		void virtual update() { }
		void virtual fixedUpdate() { }
		void virtual lateFixedUpdate() { }
		void virtual lateUpdate() { }
		void virtual onEnable() { }
		void virtual onDisable() { }

		void virtual onCollisionEnter() { }
		void virtual onCollisionStay() { }
		void virtual onCollisionExit() { }

		GameObject& getHost();
		// TODO: Add collision and trigger functions
		//void virtual onCollisions()
	};
}