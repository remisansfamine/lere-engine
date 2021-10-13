#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "pyhelper.hpp"

namespace Resources
{
	class Script
	{
	private:
		std::string scriptPath;

		CPyObject pyModule;
		CPyObject pyDict;

		std::vector<std::unique_ptr<CPyObject>> instances;

		std::unordered_map<std::string, CPyObject> functions;
		std::unordered_map<std::string, CPyObject> classes;
		std::unordered_map<std::string, CPyObject> classFunctions;

		void addFunction(const std::string& functionName);

	public:
		Script(const std::string& scriptPath);

		void initializeModule();
		void initializeDictionary();

		void killModule();

		void reload();

		CPyObject callFunction(const std::string& functionName);

		void callFunction(CPyObject* instance, const std::string& functionName, const char* format = nullptr, ...);

		CPyObject* createClassInstance(const std::string& classToInitialize);

		std::string& getScriptPath();
	};
}