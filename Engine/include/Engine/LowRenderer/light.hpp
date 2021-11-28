#pragma once

#include "component.hpp"
#include "shader.hpp"
#include "color.hpp"

namespace Physics
{
	class TransformComponent;
}

namespace LowRenderer
{
	class Shadow;

	class Light : public Engine::Component
	{
	private:
		Physics::TransformComponent* m_transform = nullptr;

		Core::Maths::mat4 spaceMatrix = Core::Maths::identity();

	public:
		Light(Engine::Entity& owner);

		std::unique_ptr<Shadow> shadow = nullptr;

		Core::Maths::vec4 position;
		Color ambient  = Color::black;
		Color diffuse  = Color::white;
		Color specular = Color::black;

		Core::Maths::vec3 attenuation = Core::Maths::vec3(1.f, 0.f, 0.f);
		float cutoff = Core::Maths::PI;
		Core::Maths::vec3 direction = Core::Maths::vec3(0.f, 0.f, -1.f);
		float outterCutoff = Core::Maths::PI;

		float enable = 1.f;
		float hasShadow = 0.f;
		
		void setAsDirectionnal();
		void setAsPoint();
		void setAsSpot();
		void setShadows(bool isShadow);
		void compute();
		void sendToProgram(std::shared_ptr<Resources::ShaderProgram> program, int index) const;

		const Core::Maths::mat4& getSpaceMatrix() const;

		void drawImGui() override;
		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}