#pragma once

#include "renderer.hpp"
#include "model.hpp"

namespace LowRenderer
{
	class ColliderRenderer : public Renderer
	{
	private:
		LowRenderer::Model model;

	public:
		ColliderRenderer(Engine::GameObject& gameObject, const std::string& shaderProgramName);
		~ColliderRenderer();

		//void draw() override;
	};
}