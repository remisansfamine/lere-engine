#pragma once

#include "pyhelper.hpp"

#include "component.hpp"

#include <string>
#include <memory>

#include "transform.hpp"
#include "rigidbody.hpp"

namespace Engine
{
	class ScriptComponent : public Component
	{
	private:
		CPyObject* instance = nullptr;

		std::string scriptName;

	public:
		Physics::Transform* transform;
		Physics::Rigidbody* rigidbody;

		ScriptComponent(Engine::GameObject& gameObject, const std::string& scriptName);

		void awake() override;
		void start() override;
		void update() override;
		void lateUpdate() override;
		void fixedUpdate() override;
		void lateFixedUpdate() override;
		void onEnable() override;
		void onDisable() override;

		void drawImGui() override;
	};
}