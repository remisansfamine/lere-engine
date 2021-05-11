#pragma once

#include <memory>

#include "sprite_renderer.hpp"

namespace Physics
{
	class Transform;
}

namespace UI
{
	class Button : public Engine::Component
	{
	private:
		std::shared_ptr<LowRenderer::SpriteRenderer> m_image = nullptr;
		std::shared_ptr<Physics::Transform> m_transform = nullptr;
	public:
		Button(Engine::GameObject& gameObject, const std::string& shaderProgramName, const std::string& texturePath);

		void update() override;

		virtual void onClick() { }
	};
}
