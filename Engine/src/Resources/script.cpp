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

		if (pyModule)
		{
			pyModule = PyImport_ReloadModule(pyModule);

			if (!pyModule)
				Core::Debug::Log::error("Cannot reload the python script " + scriptPath);
		}
		else
		{
			pName = PyUnicode_FromString(scriptMod);
			pyModule = PyImport_ImportModule(scriptMod);

			if (!pyModule)
				Core::Debug::Log::error("Cannot load the python script " + scriptPath);
		}

		pyDict = PyModule_GetDict(pyModule);

		initializeFunctions();

	}

	void Script::initializeFunctions()
	{
		PyObject* pyKey = nullptr, *pyValue = nullptr;
		for (Py_ssize_t i = 0; PyDict_Next(pyDict, &i, &pyKey, &pyValue);)
		{
			const char* key = PyUnicode_AsUTF8(pyKey);

			if (PyFunction_Check(pyValue))
				functions[std::string(key)] = pyValue;
		}
	}

	void Script::initializeClass()
	{
		if (pyClass)
			return;

		const char* scriptMod = scriptPath.c_str();


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

	CPyObject Script::callFunction(const std::string& functionName)
	{
		auto functionIt = functions.find(functionName);

		void* ptr = nullptr;

		if (functionIt == functions.end())
			return nullptr;

		return PyObject_CallObject(functionIt->second, nullptr);
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

	std::string& Script::getScriptPath()
	{
		return scriptPath;
	}
}