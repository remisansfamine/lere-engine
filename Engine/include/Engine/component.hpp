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

		void onDestroy() override;

		template <class C, class ...Crest, typename B = std::enable_if_t<std::is_base_of<Component, C>::value>>
		std::shared_ptr<C> requireComponent(Crest... args)
		{
			std::shared_ptr<C> tempPtr;

			if (!m_gameObject.tryGetComponent<C>(tempPtr))
				return m_gameObject.addComponent<C>(args...);

			return tempPtr;
		}

	public:
		bool hasStarted = false;

		void setActive(bool value) override;

		virtual void draw() const { }
		virtual void drawImGui();

		virtual void awake() { }
		virtual void start() { }
		virtual void update() { }
		virtual void fixedUpdate() { }
		virtual void lateFixedUpdate() { }
		virtual void lateUpdate() { }
		virtual void onEnable() { }
		virtual void onDisable() { }

		bool isActive() override;
		void destroy() override;

		virtual void onCollisionEnter(const Physics::Collision& collision) {}
		virtual void onCollisionStay(const Physics::Collision& collision) {}
		virtual void onCollisionExit(const Physics::Collision& collision) {}

		virtual void onTriggerEnter(Physics::Collider* collider) {}
		virtual void onTriggerStay(Physics::Collider* collider) {}
		virtual void onTriggerExit(Physics::Collider* collider) {}

		GameObject& getHost();

		virtual std::string toString() const { return ""; }
	};
}