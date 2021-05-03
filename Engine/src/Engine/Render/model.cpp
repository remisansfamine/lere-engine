#include "model.hpp"

#include "transform.hpp"
#include "resources_manager.hpp"
#include "render_manager.hpp"

namespace LowRenderer
{
	Model::Model(const std::string& filePath, std::shared_ptr<Physics::Transform>& transform, const std::string& shaderProgramName)
		: m_transform(transform), m_shaderProgram(Resources::ResourcesManager::loadShaderProgram(shaderProgramName)), m_filePath(filePath)
	{
		// TODO: Load from Resoures Manager
		Resources::ResourcesManager::loadObj(filePath);

		std::vector<std::string>* modelChildrens = Resources::ResourcesManager::getMeshNames(filePath);

		if (modelChildrens != nullptr)
		{
			for (std::string& meshName : *modelChildrens)
			{
				Model child = Model(transform, meshName, m_shaderProgram);
				child.m_material = Resources::ResourcesManager::getMatByMeshName(meshName);
				m_children.push_back(child);
			}
		}

		m_mesh = nullptr;
	}

	Model::Model(std::shared_ptr<Physics::Transform>& transform, const std::string& meshName, std::shared_ptr<Resources::ShaderProgram> program)
		: m_transform(transform), m_shaderProgram(program), m_mesh(Resources::ResourcesManager::getMeshByName(meshName))
	{ }

	void Model::draw()
	{
		if (m_mesh)
		{
			m_shaderProgram->bind();

			m_shaderProgram->setUniform("model", m_transform->getModel().e, 1, 1);

			std::shared_ptr<Camera> cam = LowRenderer::RenderManager::getCurrentCamera();

			if (cam != nullptr)
				m_shaderProgram->setUniform("viewProj", cam->getViewProjection().e, 1, 1);

			// use other material
			m_material->sendToShader(m_shaderProgram);
			m_material->bindTextures();

			m_mesh->draw();

			m_shaderProgram->unbind();
		}

		for (Model& child : m_children)
			child.draw();
	}

	std::string& Model::getPath()
	{
		return m_filePath;
	}
}