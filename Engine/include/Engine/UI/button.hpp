#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>

#include "sprite_renderer.hpp"

namespace Physics
{
	class TransformComponent;
}

namespace UI
{
	enum class ButtonState
	{
		HIGHLIGHT,
		DOWN,
		STAY,
		UP
	};

	class Button : public Engine::Component
	{
	private:
		LowRenderer::SpriteRenderer* m_image = nullptr;
		Physics::TransformComponent* m_transform = nullptr;
		std::unordered_map<ButtonState, std::vector<std::function<void()>>> functions;

		void onClick();
		void onHighlight();
		void onClickStay();
		void onClickRelease();

	public:
		Button(Engine::Entity& owner, const std::string& shaderProgramName, const std::string& texturePath);

		void update() override;

		void addListener(ButtonState state, std::function<void()> function);

		void drawImGui() override;
		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);

		LowRenderer::SpriteRenderer* getSprite();
	};
}
