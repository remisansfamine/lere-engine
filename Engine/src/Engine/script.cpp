#include "script.hpp"

#include "resources_manager.hpp"

#include "debug.hpp"

#include "utils.hpp"

namespace Resources
{
	Script::Script(const std::string& scriptPath)
		: scriptPath(scriptPath)
	{
	}

	void Script::initializeClass()
	{
		const char* scriptMod = scriptPath.c_str();

		void* ptr = nullptr;

		if (pyModule)
		{
			pyModule = PyImport_ReloadModule(pyModule);

			if (!pyModule)
				Core::Debug::Log::error("Cannot reload the python script " + scriptPath);
		}
		else
		{
			pName = PyUnicode_FromString(scriptMod);
			pyModule = PyImport_Import(pName);

			if (!pyModule)
				Core::Debug::Log::error("Cannot load the python script " + scriptPath);
		}


		pyDict = PyModule_GetDict(pyModule);

		pyClass = PyDict_GetItemString(pyDict, scriptMod);

		if (!PyCallable_Check(pyClass))
			Core::Debug::Log::error(scriptPath + " class not found");
	}

	void Script::reload()
	{
		Core::Debug::Log::info(std::string("Reloading ") + scriptPath + std::string(" script"));

		initializeClass();
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

		if (functionIt == functions.end() || !PyCallable_Check(functionIt->second.get()))
			return;

		PyObject_CallObject(functionIt->second, nullptr);
	}

	void Script::callFunction(CPyObject& instance, const std::string& functionName)
	{
		PyObject_CallMethod(instance, functionName.c_str(), nullptr);
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

		instance = script->createClassInstance();

		void* ptr = nullptr;
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