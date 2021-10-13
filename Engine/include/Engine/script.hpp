#pragma once

#include "pyhelper.hpp"

#include "component.hpp"

#include <string>
#include <memory>
#include <unordered_map>

#include "transform.hpp"
#include "rigidbody.hpp"

namespace Resources
{
	class Script
	{
	private:
		std::string scriptPath;

		CPyObject pName;
		CPyObject pyModule;
		CPyObject pyDict;

		std::vector<std::unique_ptr<CPyObject>> instances;

		std::unordered_map<std::string, CPyObject> functions;

		void addFunction(const std::string& functionName);

	public:
		CPyObject pyClass;

		Script(const std::string& scriptPath);

		void initializeModule();
		void initializeFunctions();
		void initializeClass();

		void killModule();

		void reload();

		void callFunction(const std::string& functionName);

		void callFunction(CPyObject* instance, const std::string& functionName, const char* format = nullptr, ...);

		CPyObject* createClassInstance();
	};
}

namespace Engine
{
	class ScriptComponent : public Component
	{
	private:
		std::shared_ptr<Resources::Script> script;

		CPyObject* instance = nullptr;

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
	};
}