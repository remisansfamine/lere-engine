#pragma once

#include "pyhelper.hpp"

#include "component.hpp"

#include <string>
#include <memory>
#include <unordered_map>

namespace Resources
{
	class Script
	{
	private:
		CPyObject pyModule;
		CPyObject pyDict;
		CPyObject pyClass;

		std::unordered_map<std::string, CPyObject> functions;

		void addFunction(const std::string& functionName);

	public:
		Script(const std::string& scriptPath);

		void callFunction(const std::string& functionName);

		void callFunction(CPyObject& instance, const std::string& functionName);

		CPyObject createClassInstance();
	};
}

namespace Engine
{
	class ScriptComponent : public Component
	{
	private:
		std::shared_ptr<Resources::Script> script;

		CPyObject instance;

	public:
		ScriptComponent(Engine::GameObject& gameObject, const std::string& scriptName);

		void awake() override;
		void start() override;
		void update() override;
		void lateUpdate() override;
		void fixedUpdate() override;
		void lateFixedUpdate() override;
		void onEnable() override;
		void onDisable() override;
	};
}