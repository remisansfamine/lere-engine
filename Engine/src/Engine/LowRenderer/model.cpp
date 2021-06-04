#include "model.hpp"

#include "imgui.h"

#include "resources_manager.hpp"
#include "render_manager.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	Model::Model(const std::string& filePath, std::shared_ptr<Physics::Transform> transform)
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

	void Model::draw(std::shared_ptr<Resources::ShaderProgram> shaderProgram) const
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
		for (const Model& child : m_children)
			child.draw(shaderProgram);
	}

	void Model::simpleDraw(std::shared_ptr<Resources::ShaderProgram> shaderProgram) const
	{
		if (m_mesh)
		{
			// Send model matrix to program
			shaderProgram->setUniform("model", m_transform->getGlobalModel().e, 1, 1);

			// Draw the mesh
			m_mesh->draw();
		}

		// Draw children
		for (const Model& child : m_children)
			child.simpleDraw(shaderProgram);
	}

	void Model::drawCollider(std::shared_ptr<Resources::ShaderProgram> shaderProgram, Core::Maths::mat4& modelCollider) const
	{
		if (m_mesh)
		{
			Core::Maths::vec3 color = Core::Maths::vec3(0.f, 1.f, 0.f);
			// Send model matrix to program
			shaderProgram->setUniform("model", modelCollider.e, 1, 1);
			shaderProgram->setUniform("color", color.e, 1, 1);

			// Draw the mesh
			m_mesh->draw();
		}

		// Draw children
		for (const Model& child : m_children)
			child.drawCollider(shaderProgram, modelCollider);
	}

	std::string Model::getPath() const
	{
		return m_filePath;
	}

	void Model::drawImGui()
	{
		ImGui::Text(m_filePath.c_str());
	}

	void Model::setDiffuseTexture(const std::string& difTexName)
	{
		
		for (auto child : m_children)
		{

		}
	}
}