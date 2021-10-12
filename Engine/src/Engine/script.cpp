#include "script.hpp"

#include "resources_manager.hpp"

#include "debug.hpp"

#include "utils.hpp"

namespace Resources
{
	Script::Script(const std::string& scriptPath)
	{
		const char* scriptMod = scriptPath.c_str();

		CPyObject pName = PyUnicode_FromString(scriptMod);

		pyModule = PyImport_Import(pName);

		if (!pyModule)
			Core::Debug::Log::error("Cannot load the python script " + scriptPath);

		pyDict = PyModule_GetDict(pyModule);

		pyClass = PyDict_GetItemString(pyDict, scriptMod);

		PyObject_CallMethod(createClassInstance(),"update", "");


		if (!PyCallable_Check(pyClass))
			Core::Debug::Log::error(scriptPath + " class not found");

		addFunction("awake");
		addFunction("start");
		addFunction("update");
		addFunction("lateUpdate");
		addFunction("fixedUpdate");
		addFunction("lateFixedUpdate");
		addFunction("onEnable");
		addFunction("onDisable");
	}

	void Script::addFunction(const std::string& functionName)
	{
		functions[functionName] = PyObject_GetAttrString(pyModule, functionName.c_str());

		if (!PyCallable_Check(functions[functionName]))
			functions.erase(functionName);
	}

	void Script::callFunction(const std::string& functionName)
	{
		auto functionIt = functions.find(functionName);

		if (functionIt == functions.end() || !PyCallable_Check(functionIt->second))
			return;

		PyObject_CallObject(functionIt->second, NULL);
	}

	void Script::callFunction(CPyObject& instance, const std::string& functionName)
	{
		PyObject_CallMethod(instance, functionName.c_str(), "");
	}

	CPyObject Script::createClassInstance()
	{
		return PyObject_CallObject(pyClass, nullptr);
	}
}

namespace Engine
{
	ScriptComponent::ScriptComponent(Engine::GameObject& gameObject, const std::string& scriptName)
		: Component(gameObject, std::shared_ptr<ScriptComponent>(this))
	{
		script = Resources::ResourcesManager::loadScript(scriptName);

		//instance = script->createClassInstance();
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

	void ScriptComponent::lateUpdate()
	{
		script->callFunction("lateUpdate");
		script->callFunction(instance, "lateUpdate");
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
}