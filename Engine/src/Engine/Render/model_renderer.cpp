#include "model_renderer.hpp"

#include "imgui.h"

#include "resources_manager.hpp"
#include "render_manager.hpp"
#include "inputs_manager.hpp"
#include "time.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	ModelRenderer::ModelRenderer(Engine::GameObject& gameObject, const std::shared_ptr<ModelRenderer>& ptr, const std::string& shaderPromgramName)
		: Renderer(gameObject, ptr, shaderPromgramName)
	{
		LowRenderer::RenderManager::linkComponent(ptr);
	}

	ModelRenderer::ModelRenderer(Engine::GameObject& gameObject, const std::string& filePath, const std::string& shaderPromgramName)
		: ModelRenderer(gameObject, std::shared_ptr<ModelRenderer>(this), shaderPromgramName)
	{
		model = Model(filePath, m_transform);
	}

	ModelRenderer::~ModelRenderer()
	{
		Core::Debug::Log::info("Unload model " + model.getPath());
	}

	void ModelRenderer::draw() const
	{
		model.draw(m_shaderProgram);
	}

	void ModelRenderer::drawImGui()
	{
		if (ImGui::TreeNode("Model renderer"))
		{
			model.drawImGui();
			ImGui::TreePop();
		}
	}

	std::string ModelRenderer::toString() const
	{
		return "COMP MODELRENDERER " + model.getPath() + " " + m_shaderProgram->getName();
	}

	void ModelRenderer::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::string modelPath, shaderProgramName;

		iss >> modelPath;
		iss >> shaderProgramName;

		gameObject.addComponent<ModelRenderer>(modelPath, shaderProgramName);
	}
}