#include "script.hpp"

#include "resources_manager.hpp"
#include "inputs_manager.hpp"

#include "debug.hpp"

#include "utils.hpp"

namespace Resources
{
	Script::Script(const std::string& scriptPath)
		: scriptPath(scriptPath)
	{
	}
	
	void Script::initializeModule()
	{
		const char* scriptMod = scriptPath.c_str();

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
	}

	void Script::initializeFunctions()
	{
		addFunction("awake");
		addFunction("start");
		addFunction("update");
		addFunction("lateUpdate");
		addFunction("fixedUpdate");
		addFunction("lateFixedUpdate");
		addFunction("onEnable");
		addFunction("onDisable");
	}

	void Script::initializeClass()
	{
		if (pyClass)
			return;

		const char* scriptMod = scriptPath.c_str();

		pyDict = PyModule_GetDict(pyModule);

		pyClass = PyDict_GetItemString(pyDict, scriptMod);

		if (!PyCallable_Check(pyClass))
			Core::Debug::Log::error(scriptPath + " class not found");
	}

	void Script::killModule()
	{
		pName.release();
		pyModule.release();
		pyDict.release();
		pyClass.release();
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

	void Script::callFunction(CPyObject* instance, const std::string& functionName, const char* format, ...)
	{
		if (!instance)
			return;

		if (!format)
		{
			PyObject_CallMethod(*instance, functionName.c_str(), format);
			return;
		}

		va_list myargs;
		va_start(myargs, format);

		PyObject_CallMethod(*instance, functionName.c_str(), format, myargs);

		va_end(myargs);
	}

	CPyObject* Script::createClassInstance()
	{
		initializeClass();

		instances.push_back(std::make_unique<CPyObject>(PyObject_CallObject(pyClass, nullptr)));

		return instances.back().get();
	}
}

namespace Engine
{
	ScriptComponent::ScriptComponent(Engine::GameObject& gameObject, const std::string& scriptName)
		: Component(gameObject, std::shared_ptr<ScriptComponent>(this))
	{
		transform = getHost().getComponent<Physics::Transform>().get();
		rigidbody = getHost().getComponent<Physics::Rigidbody>().get();

		script = Resources::ResourcesManager::loadScript(scriptName);
		script->initializeFunctions();
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