#include "model.hpp"

#include "resources_manager.hpp"
#include "render_manager.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	Model::Model(const std::string& filePath, std::shared_ptr<Physics::Transform>& transform)
		: m_transform(transform), m_filePath(filePath)
	{
		// Load meshes
		Resources::ResourcesManager::loadObj(filePath);

		std::vector<std::string>* modelChildrens = Resources::ResourcesManager::getMeshNames(filePath);

		// Get model childrens
		if (modelChildrens != nullptr)
		{
			for (std::string& meshName : *modelChildrens)
			{
				Model child = Model(transform, meshName);

				std::shared_ptr<Resources::Material> newMat = Resources::ResourcesManager::getMatByMeshName(meshName);

				if (newMat)
					child.m_material = newMat;
				m_children.push_back(child);
			}
		}

		m_mesh = nullptr;
	}

	Model::Model(std::shared_ptr<Physics::Transform>& transform, const std::string& meshName)
		: m_transform(transform), m_mesh(Resources::ResourcesManager::getMeshByName(meshName))
	{ }

	void Model::draw(std::shared_ptr<Resources::ShaderProgram> shaderProgram)
	{
		if (m_mesh)
		{
			// Send model matrix to program
			shaderProgram->setUniform("model", m_transform->getGlobalModel().e, 1, 1);

			// Send and bind material to program
			m_material->sendToShader(shaderProgram);
			m_material->bindTextures();

			// Draw the mesh
			m_mesh->draw();
		}

		// Draw children
		for (Model& child : m_children)
			child.draw(shaderProgram);
	}

	std::string& Model::getPath()
	{
		return m_filePath;
	}

	void Model::drawImGui()
	{

	}
}