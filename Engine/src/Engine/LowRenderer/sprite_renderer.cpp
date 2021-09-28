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
	SpriteRenderer::SpriteRenderer(Engine::GameObject& gameObject, const std::shared_ptr<SpriteRenderer>& ptr, const std::string& shaderPromgramName)
		: Renderer(gameObject, ptr, shaderPromgramName)
	{
		LowRenderer::RenderManager::linkComponent(ptr);
	}

	SpriteRenderer::SpriteRenderer(Engine::GameObject& gameObject, const std::string& shaderPromgramName, const Core::Maths::vec2& tilling)
		: SpriteRenderer(gameObject, std::shared_ptr<SpriteRenderer>(this), shaderPromgramName)
	{ 
		mesh = Resources::ResourcesManager::getMeshByName("Plane");
		texture = Resources::Texture::defaultDiffuse;

		tillingMultiplier = tilling.x;
		tillingOffset = tilling.y;
	}

	SpriteRenderer::SpriteRenderer(Engine::GameObject& gameObject, const std::string& shaderProgramName, const std::string& texturePath, const Core::Maths::vec2& tilling)
		: SpriteRenderer(gameObject, std::shared_ptr<SpriteRenderer>(this), shaderProgramName)
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

		m_shaderProgram->setUniform("model", m_transform->getGlobalModel().e, 1, 1);
		m_shaderProgram->setUniform("color", m_color.e);

		m_shaderProgram->setUniform("tilling", Core::Maths::vec2(tillingMultiplier, tillingOffset).e);

		int i = 0;
		m_shaderProgram->setUniform("diffuseTex", &i);

		if (texture)
			if (!texture->bind(0))
				Resources::Texture::defaultDiffuse->bind(0);

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

	void SpriteRenderer::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::string texturePath, shaderProgramName;
		Core::Maths::vec2 tilling;

		iss >> shaderProgramName;
		iss >> texturePath;

		iss >> tilling.x;
		iss >> tilling.y;

		std::shared_ptr<SpriteRenderer> sprite;
		if (!gameObject.tryGetComponent<SpriteRenderer>(sprite))
		{
			gameObject.addComponent<SpriteRenderer>(shaderProgramName, texturePath, tilling);
			return;
		}

		sprite->m_shaderProgram = Resources::ResourcesManager::loadShaderProgram(shaderProgramName);
		sprite->texture = Resources::ResourcesManager::loadTexture(texturePath);
		sprite->tillingMultiplier = tilling.x;
		sprite->tillingOffset = tilling.y;
	}
}