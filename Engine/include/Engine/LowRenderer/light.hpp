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

	struct LightData
	{
		// 16 f : 1 m
		Core::Maths::vec3 position;
		float isPoint;
		Color ambient = Color::black;
		Color diffuse = Color::white;
		Color specular = Color::black;

		// 8 f : 1/2 m
		Core::Maths::vec3 attenuation = Core::Maths::vec3(1.f, 0.f, 0.f);
		float cutoff = Core::Maths::PI;
		Core::Maths::vec3 direction = Core::Maths::vec3(0.f, 0.f, -1.f);
		float outterCutoff = Core::Maths::PI;

		float enable = 1.f;
		float hasShadow = 0.f;

		Core::Maths::mat4 spaceMatrix = Core::Maths::identity();
	};

	class Light : public Engine::Component, public LightData
	{
	private:
		Physics::TransformComponent* m_transform = nullptr;

	public:
		Light(Engine::Entity& owner);

		std::unique_ptr<Shadow> shadow = nullptr;
		
		void setAsDirectionnal();
		void setAsPoint();
		void setAsSpot();
		void setShadows(bool isShadow);
		void compute();
		void sendToProgram(std::shared_ptr<Resources::ShaderProgram> program, int index) const;
		void addToLightBuffer(std::vector<LightData>& buffer);


		const Core::Maths::mat4& getSpaceMatrix() const;

		void drawImGui() override;
		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}