#pragma once

#include "renderer.hpp"
#include <string>

namespace LowRenderer
{
	class SpriteRenderer : public Renderer
	{
	private:
	// std::shared_ptr<Texture> texture = nullptr;
		std::string lol = "0";

	public:
		SpriteRenderer(Engine::GameObject& gameObject);

		void draw() override;
		void update() override;

	};
}