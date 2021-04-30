#pragma once

#include "renderer.hpp"
#include "model.hpp"

namespace LowRenderer
{
	class ModelRenderer : public Renderer
	{
	private:
		LowRenderer::Model model;

	public:
		ModelRenderer(Engine::GameObject& gameObject, const std::string& filePath);
		~ModelRenderer();

		void draw() override;
		void update() override;
	};
}