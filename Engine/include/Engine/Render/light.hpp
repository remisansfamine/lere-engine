#pragma once

#include "component.hpp"
#include "color.hpp"

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
		Color diffuse  = Color::white;
		Color specular = Color::black;
		Color ambient  = Color::black;

		Core::Maths::vec3 direction;
		float cutoff = Core::Maths::PI;
		Core::Maths::vec3 attenuation;
		float outterCutoff = Core::Maths::PI;

		float enable = 1.f;
		
		void setAsDirectionnal();
		void setAsPoint();
		void setAsSpot();
		void compute();

		void draw() override;
	};
}