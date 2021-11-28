#include "model_renderer.hpp"

#include "imgui.h"

#include "resources_manager.hpp"
#include "render_manager.hpp"
#include "inputs_manager.hpp"
#include "time.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	ModelRenderer::ModelRenderer(Engine::Entity& owner, const std::string& filePath, const std::string& shaderPromgramName, const Core::Maths::vec2& tilling)
		: Renderer(owner, shaderPromgramName), model(filePath, m_transform)
	{
		LowRenderer::RenderManager::linkComponent(this);

		tillingMultiplier = tilling.x;
		tillingOffset = tilling.y;
	}

	ModelRenderer::~ModelRenderer()
	{
		Core::Debug::Log::info("Unload model " + model.getPath());
	}

	void ModelRenderer::onDestroy()
	{
		Component::onDestroy();

		LowRenderer::RenderManager::removeComponent(this);
	}

	void ModelRenderer::draw() const
	{
		m_shaderProgram->setUniform("tilling", Core::Maths::vec2(tillingMultiplier, tillingOffset).e, false);

		model.draw(m_shaderProgram);
	}

	void ModelRenderer::simpleDraw(std::shared_ptr<Resources::ShaderProgram> program) const
	{
		model.simpleDraw(program);
	}

	void ModelRenderer::drawImGui()
	{
		if (ImGui::TreeNode("Model renderer"))
		{
			ImGui::Text("Textures :");

			ImGui::DragFloat("TillingMultiplier", &tillingMultiplier, 0.1f, 0.f, 200.f);
			ImGui::DragFloat("TillingOffset", &tillingOffset, 0.01f, 0.f, 1.f);

			model.drawImGui();
			Component::drawImGui();

			ImGui::TreePop();
		}
	}

	std::string ModelRenderer::toString() const
	{
		return "COMP MODELRENDERER " + model.getPath() + " " + m_shaderProgram->getName() + " " + std::to_string(tillingMultiplier) + " " + std::to_string(tillingOffset);
	}

	void ModelRenderer::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		std::string modelPath, shaderProgramName;
		Core::Maths::vec2 tilling;

		iss >> modelPath;
		iss >> shaderProgramName;
		iss >> tilling.x;
		iss >> tilling.y;

		owner.addComponent<ModelRenderer>(modelPath, shaderProgramName, tilling);
	}
}