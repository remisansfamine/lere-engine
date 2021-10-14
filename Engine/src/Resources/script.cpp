#include "script.hpp"

#include "debug.hpp"

namespace Resources
{
	Script::Script(const std::string& scriptPath)
		: scriptPath(scriptPath)
	{
	}

	void Script::initializeModule()
	{
		const char* scriptMod = scriptPath.c_str();

		functions.clear();
		classes.clear();

		pyModule = pyModule ? PyImport_ReloadModule(pyModule) : PyImport_ImportModule(scriptMod);

		if (!pyModule)
			Core::Debug::Log::error("Cannot load the python script " + scriptPath);

		initializeDictionary();
	}

	void Script::initializeDictionary()
	{
		pyDict = PyModule_GetDict(pyModule);

		PyObject* pyKey = nullptr, *pyValue = nullptr;
		for (Py_ssize_t i = 0; PyDict_Next(pyDict, &i, &pyKey, &pyValue);)
		{
			const char* key = PyUnicode_AsUTF8(pyKey);

			if (PyFunction_Check(pyValue))
			{
				functions[std::string(key)] = pyValue;
				continue;
			}

			if (PyType_Check(pyValue))
			{
				classes[std::string(key)] = pyValue;
				continue;
			}
		}
	}

	void Script::killModule()
	{
		pyModule.release();
		pyDict.release();

		functions.clear();
		classes.clear();
	}

	void Script::reload()
	{
		Core::Debug::Log::info(std::string("Reloading ") + scriptPath + std::string(" script"));
	}

	void Script::addFunction(const std::string& functionName)
	{
		functions[functionName] = PyObject_GetAttrString(pyModule, functionName.c_str());

		if (!PyCallable_Check(functions[functionName]))
			functions.erase(functionName);
	}

	CPyObject Script::callFunction(const std::string& functionName)
	{
		auto functionIt = functions.find(functionName);

		void* ptr = nullptr;

		if (functionIt == functions.end())
			return nullptr;

		return PyObject_CallFunction(functionIt->second, nullptr);
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

	CPyObject* Script::createClassInstance(const std::string& className)
	{
		auto classIt = classes.find(className);

		if (classIt == classes.end())
			return nullptr;

		instances.push_back(std::make_unique<CPyObject>(PyObject_CallObject(classIt->second, nullptr)));

		return instances.back().get();
	}

	std::string& Script::getScriptPath()
	{
		return scriptPath;
	}
}