#pragma once

#include "renderer.hpp"

namespace LowRenderer
{
	class SpriteRenderer : public Renderer
	{
	private:
		// std::shared_ptr<Texture> texture = nullptr;

	public:
		SpriteRenderer(Engine::GameObject& gameObject, const std::string& shaderPromgramName);

		void draw() const override;
		//void drawImGui() override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}