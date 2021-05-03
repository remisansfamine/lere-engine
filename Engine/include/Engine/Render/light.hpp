#pragma once

#include "component.hpp"
#include "color.hpp"
#include "shader.hpp"

namespace Physics
{
	class Transform;
}

namespace LowRenderer
{
	class Light : public Engine::Component
	{
	private:
		Light(Engine::GameObject& gameObject, const std::shared_ptr<Light>& ptr);

		std::shared_ptr<Physics::Transform> m_transform = nullptr;

	public:
		Light(Engine::GameObject& gameObject);

		Core::Maths::vec4 position;
		Color ambient  = Color::black;
		Color diffuse  = Color::white;
		Color specular = Color::black;

		Core::Maths::vec3 attenuation = Core::Maths::vec3(1.f, 0.f, 0.f);
		float cutoff = Core::Maths::PI;
		Core::Maths::vec3 direction = Core::Maths::vec3(0.f, 0.f, -1.f);
		float outterCutoff = Core::Maths::PI;

		float enable = 1.f;
		
		void setAsDirectionnal();
		void setAsPoint();
		void setAsSpot();
		void compute();
		void sendToProgram(std::shared_ptr<Resources::ShaderProgram> program, int index);
	};
}