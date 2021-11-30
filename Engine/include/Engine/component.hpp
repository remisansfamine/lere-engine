#pragma once

#include <sstream>

#include "object.hpp"
#include "entity.hpp"
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
		Entity& owner;

	protected:
		Component(Entity& owner);

		void onDestroy() override;

		template <class C, class ...Crest, typename B = std::enable_if_t<std::is_base_of<Component, C>::value>>
		C* requireComponent(Crest&&... args)
		{
			C* tempPtr;

			if (!owner.tryGetComponent(tempPtr))
				return owner.addComponent<C>(args...);

			return tempPtr;
		}

	public:
		virtual ~Component() = default;

		bool hasStarted = false;

		void setActive(bool value) override;

		void virtual drawImGui();

		void virtual awake() { }
		void virtual start() { }
		void virtual update() { }
		void virtual fixedUpdate() { }
		void virtual lateFixedUpdate() { }
		void virtual lateUpdate() { }
		void virtual onEnable() { }
		void virtual onDisable() { }


		bool isActive() const override;
		void destroy() override;

		void virtual onCollisionEnter(const Physics::Collision& collision) {}
		void virtual onCollisionStay(const Physics::Collision& collision) {}
		void virtual onCollisionExit(const Physics::Collision& collision) {}

		void virtual onTriggerEnter(Physics::Collider* collider) {}
		void virtual onTriggerStay(Physics::Collider* collider) {}
		void virtual onTriggerExit(Physics::Collider* collider) {}

		Entity& getHost();
		const Entity& getHost() const;

		virtual std::string toString() const { return ""; }
	};
}