#include "model_renderer.hpp"

#include "inputs_manager.hpp"
#include "resources_manager.hpp"
#include "time.hpp"
#include "transform.hpp"

namespace LowRenderer
{
	ModelRenderer::ModelRenderer(Engine::GameObject& gameObject, const std::string& filePath)
		: Renderer(gameObject, std::shared_ptr<ModelRenderer>(this)), model("resources/obj/craftsman/craftsman.obj", m_transform, "advancedLighting")
	{
	}

	ModelRenderer::~ModelRenderer()
	{
		Core::Debug::Log::info("Unload model " + model.getPath());
	}

	void ModelRenderer::draw()
	{
		model.draw();
	}

	void ModelRenderer::update()
	{
		model.m_transform->m_position.x += Core::Input::InputManager::getAxis("Horizontal") * Core::TimeManager::getDeltaTime();
	}
}