#pragma once

#include <memory>
#include <sstream>

#include "object.hpp"
#include "game_object.hpp"
#include "collision.hpp"

namespace Physics
{
	class Collider;
}

namespace Engine
{
	class Component : public Object
	{
	private:
		GameObject& m_gameObject;

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
		bool hasStarted = false;

		void setActive(bool value) override;
		void destroy() override;

		void virtual draw() const { }
		void virtual drawImGui() {}

		void virtual awake() { }
		void virtual start() { }
		void virtual update() { }
		void virtual fixedUpdate() { }
		void virtual lateFixedUpdate() { }
		void virtual lateUpdate() { }
		void virtual onEnable() { }
		void virtual onDisable() { }

		void virtual onCollisionEnter(const Physics::Collision& collision) {}
		void virtual onCollisionStay(const Physics::Collision& collision) {}
		void virtual onCollisionExit(const Physics::Collision& collision) {}

		GameObject& getHost();

		// TODO: Add collision and trigger functions
		//void virtual onCollisions()
		virtual std::string toString() const { return ""; }
	};
}