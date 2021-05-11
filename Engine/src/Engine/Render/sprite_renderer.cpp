#include "sprite_renderer.hpp"

#include <imgui.h>

#include "render_manager.hpp"
#include "resources_manager.hpp"
#include "inputs_manager.hpp"

#include "transform.hpp"
#include "texture.hpp"

namespace LowRenderer
{
	SpriteRenderer::SpriteRenderer(Engine::GameObject& gameObject, const std::shared_ptr<SpriteRenderer>& ptr, const std::string& shaderPromgramName)
		: Renderer(gameObject, ptr, shaderPromgramName)
	{
		LowRenderer::RenderManager::linkComponent(ptr);
	}

	SpriteRenderer::SpriteRenderer(Engine::GameObject& gameObject, const std::string& shaderPromgramName)
		: SpriteRenderer(gameObject, std::shared_ptr<SpriteRenderer>(this), shaderPromgramName)
	{ 
		mesh = Resources::ResourcesManager::getMeshByName("Plane");
		texture = Resources::Texture::defaultDiffuse;
	}

	SpriteRenderer::SpriteRenderer(Engine::GameObject& gameObject, const std::string& shaderPromgramName, const std::string& texturePath)
		: SpriteRenderer(gameObject, std::shared_ptr<SpriteRenderer>(this), shaderPromgramName)
	{
		texture = texturePath == "" ? Resources::Texture::defaultDiffuse : Resources::ResourcesManager::loadTexture(texturePath);
		mesh = Resources::ResourcesManager::getMeshByName("Plane");
	}

	void SpriteRenderer::draw() const
	{
		if (mesh)
		{
			m_shaderProgram->setUniform("model", m_transform->getModel().e, 1, 1);
			m_shaderProgram->setUniform("color", m_color.e);

			int i = 0;
			m_shaderProgram->setUniform("diffuseTex", &i);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->getID());

			mesh->draw();
		}
	}

	void SpriteRenderer::drawImGui()
	{
		if (ImGui::TreeNode("Sprite renderer"))
		{
			ImGui::TreePop();
		}
	}

	std::string SpriteRenderer::toString()
	{
		return "COMP SPRITERENDERER " + m_shaderProgram->m_name + " " + texture->getPath();
	}

	void SpriteRenderer::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::string texturePath, shaderProgramName;

		iss >> shaderProgramName;
		iss >> texturePath;

		gameObject.addComponent<SpriteRenderer>(shaderProgramName, texturePath);
	}
}