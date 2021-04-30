#pragma once

#include "renderer.hpp"

namespace LowRenderer
{
	class SpriteRenderer : public Renderer
	{
	private:
		// std::shared_ptr<Texture> texture = nullptr;

	public:
		SpriteRenderer(Engine::GameObject& gameObject);

		void draw() override;
		void update() override;
	};
}