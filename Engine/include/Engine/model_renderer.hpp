#pragma once

#include "renderer.hpp"

namespace LowRenderer
{
	class ModelRenderer : public Renderer
	{
	private:
		// std::shared_ptr<Texture> texture = nullptr;

	public:
		ModelRenderer(Engine::GameObject& gameObject);

		void draw() override;
		void update() override;
	};
}