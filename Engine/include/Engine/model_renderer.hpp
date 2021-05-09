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
		ModelRenderer(Engine::GameObject& gameObject, const std::string& filePath, const std::string& shaderPromgramName);
		~ModelRenderer();

		void draw() const override;
		void drawImGui() override;
		std::string toString() override;
	};
}