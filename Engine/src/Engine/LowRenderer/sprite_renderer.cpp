#include "sprite_renderer.hpp"

#include <imgui.h>

#include "render_manager.hpp"
#include "resources_manager.hpp"
#include "inputs_manager.hpp"

#include "transform.hpp"
#include "utils.hpp"
#include "texture.hpp"

namespace LowRenderer
{
	SpriteRenderer::SpriteRenderer(Engine::Entity& owner,const std::string& shaderProgramName)
		: Renderer(owner, shaderProgramName)
	{
		LowRenderer::RenderManager::linkComponent(this);
	}

	SpriteRenderer::SpriteRenderer(Engine::Entity& owner, const std::string& shaderProgramName, const Core::Maths::vec2& tilling)
		: SpriteRenderer(owner, shaderProgramName)
	{ 
		mesh = Resources::ResourcesManager::getMeshByName("Plane");
		texture = Resources::Texture::defaultDiffuse;

		tillingMultiplier = tilling.x;
		tillingOffset = tilling.y;
	}

	SpriteRenderer::SpriteRenderer(Engine::Entity& owner, const std::string& shaderProgramName, const std::string& texturePath, const Core::Maths::vec2& tilling)
		: SpriteRenderer(owner, shaderProgramName)
	{
		texture = Resources::ResourcesManager::loadTexture(texturePath);
		mesh = Resources::ResourcesManager::getMeshByName("Plane");

		tillingMultiplier = tilling.x;
		tillingOffset = tilling.y;
	}

	void SpriteRenderer::onDestroy()
	{
		Component::onDestroy();

		LowRenderer::RenderManager::removeComponent(this);
	}

	std::string SpriteRenderer::getTexturePath()
	{
		return texture->getPath();
	}

	void SpriteRenderer::draw() const
	{
		if (!mesh)
			return;

		m_shaderProgram->setUniform("model", m_transform->getGlobalModel().e, false, 1, 1);
		m_shaderProgram->setUniform("color", m_color.e, false);

		m_shaderProgram->setUniform("tilling", Core::Maths::vec2(tillingMultiplier, tillingOffset).e, false);

		if (texture)
			if (!m_shaderProgram->setSampler("diffuseTex", texture->getID()))
				m_shaderProgram->setSampler("diffuseTex", Resources::Texture::defaultDiffuse->getID());

		mesh->draw();
	}

	void SpriteRenderer::drawImGui()
	{
		ImGui::PushID(texture->getID());

		if (ImGui::TreeNode("Sprite renderer"))
		{
			if (ImGui::CollapsingHeader("Texture:"))
			{
				std::string texStr = "Filepath: " + texture->getPath();
				const char* texText = (texStr).c_str();
				ImGui::Text(texText);

				texture->drawImGui();
			}

			ImGui::DragFloat("TillingMultiplier", &tillingMultiplier, 0.1f, 0.f, 200.f);
			ImGui::DragFloat("TillingOffset", &tillingOffset, 0.01f, 0.f, 1.f);
			texture->drawImGui();

			Component::drawImGui();

			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	std::string SpriteRenderer::toString() const
	{
		return "COMP SPRITERENDERER " +  m_shaderProgram->getName() + " " + texture->getPath() + " " + std::to_string(tillingMultiplier) + " " + std::to_string(tillingOffset);
	}

	void SpriteRenderer::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		std::string texturePath, shaderProgramName;
		Core::Maths::vec2 tilling;

		iss >> shaderProgramName;
		iss >> texturePath;

		iss >> tilling.x;
		iss >> tilling.y;

		SpriteRenderer* sprite;
		if (!owner.tryGetComponent<SpriteRenderer>(sprite))
		{
			owner.addComponent<SpriteRenderer>(shaderProgramName, texturePath, tilling);
			return;
		}

		sprite->texture = Resources::ResourcesManager::loadTexture(texturePath);
		sprite->m_shaderProgram = Resources::ResourcesManager::loadShaderProgram(shaderProgramName);
		sprite->tillingMultiplier = tilling.x;
		sprite->tillingOffset = tilling.y;
	}
}