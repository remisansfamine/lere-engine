#pragma once

#include "maths.hpp"

#include "component.hpp"

#include "game_object.hpp"

#include "model.hpp"

namespace Physics
{
	class Transform : public Engine::Component
	{
	private:
		std::shared_ptr<Transform> parent = nullptr;

	public:
		Transform(Engine::GameObject& gameObject);

		Core::Maths::vec3 m_position	= Core::Maths::vec3(0.f, 0.f, 0.f);
		Core::Maths::vec3 m_rotation	= Core::Maths::vec3(0.f, 0.f, 0.f);
		Core::Maths::vec3 m_scale		= Core::Maths::vec3(1.f, 1.f, 1.f);

		Engine::GameObject& getGOParent();
		Core::Maths::mat4 getModel() const;
		Core::Maths::mat4 getGlobalModel() const;
		Core::Maths::mat4 getParentModel() const;

		bool hasParent();
		void setParent(std::shared_ptr<Physics::Transform> _parent);
		void setParent(Engine::GameObject& gameObject);
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss, std::string& parentName);
	};
}