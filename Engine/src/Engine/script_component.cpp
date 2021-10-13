#include "script_component.hpp"

#include "resources_manager.hpp"
#include "inputs_manager.hpp"

#include "debug.hpp"
#include <imgui.h>

#include "utils.hpp"


namespace Engine
{
	ScriptComponent::ScriptComponent(Engine::GameObject& gameObject, const std::string& scriptName)
		: Component(gameObject, std::shared_ptr<ScriptComponent>(this)), scriptName(scriptName)
	{
		transform = getHost().getComponent<Physics::Transform>().get();
		rigidbody = getHost().getComponent<Physics::Rigidbody>().get();

		script = Resources::ResourcesManager::loadScript(scriptName);
		//script->initializeFunctions();
		instance = script->createClassInstance(scriptName);
	}

	void ScriptComponent::awake()
	{
		script->callFunction("awake");
		script->callFunction(instance, "awake");
	}

	void ScriptComponent::start()
	{
		script->callFunction("start");
		script->callFunction(instance, "start");
	}

	void ScriptComponent::update()
	{
		script->callFunction("update");
		script->callFunction(instance, "update");
	}

	void ScriptComponent::lateUpdate()
	{
		script->callFunction("lateUpdate");
		script->callFunction(instance, "lateUpdate");
	}

	void ScriptComponent::fixedUpdate()
	{
		script->callFunction("fixedUpdate");
		script->callFunction(instance, "fixedUpdate");
	}

	void ScriptComponent::lateFixedUpdate()
	{
		script->callFunction("lateFixedUpdate");
		script->callFunction(instance, "lateFixedUpdate");
	}

	void ScriptComponent::onEnable()
	{
		script->callFunction("onEnable");
		script->callFunction(instance, "onEnable");
	}

	void ScriptComponent::onDisable()
	{
		script->callFunction("onDisable");
		script->callFunction(instance, "onDisable");
	}

	void ScriptComponent::drawImGui()
	{
		if (ImGui::TreeNode(script->getScriptPath().c_str()))
		{
			ImGui::TreePop();
		}
	}
}