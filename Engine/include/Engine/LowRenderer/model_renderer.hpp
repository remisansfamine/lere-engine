#pragma once

#include "renderer.hpp"
#include "model.hpp"

namespace LowRenderer
{
	class ModelRenderer : public Renderer
	{
	private:
		LowRenderer::Model model;

		float tillingMultiplier = 1.f;
		float tillingOffset = 0.f;

		void onDestroy() override;

	public:
		ModelRenderer(Engine::Entity& owner, const std::string& filePath, const std::string& shaderPromgramName, const Core::Maths::vec2& tilling = Core::Maths::vec2(1.f, 0.f));
		~ModelRenderer();

		void draw() const override;
		void simpleDraw(std::shared_ptr<Resources::ShaderProgram> program) const;
		void drawImGui() override;
		std::string toString() const override;

		static void parseComponent(Engine::Entity& owner, std::istringstream& iss);
	};
}