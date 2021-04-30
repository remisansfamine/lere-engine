#include "model_renderer.hpp"

namespace LowRenderer
{
	ModelRenderer::ModelRenderer(Engine::GameObject& gameObject)
		: Renderer(gameObject, std::shared_ptr<ModelRenderer>(this))
	{ }

	void ModelRenderer::draw()
	{

	}

	void ModelRenderer::update()
	{

	}
}