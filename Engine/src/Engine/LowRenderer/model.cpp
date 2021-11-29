#include "model.hpp"

#include "imgui.h"

#include "utils.hpp"

#include "resources_manager.hpp"
#include "render_manager.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	Model::Model(const std::string& filePath, Physics::TransformComponent* transform)
		: m_transform(transform), m_filePath(filePath), m_name(Utils::getFileNameFromPath(filePath))
	{
		Resources::ResourcesManager::manageTask(&Model::loadMeshes, this);
	}

	Model::Model(Physics::TransformComponent* transform, const std::string& meshName)
		: m_transform(transform), m_mesh(Resources::ResourcesManager::getMeshByName(meshName)), m_name(meshName)
	{
	
	}

	void Model::loadMeshes()
	{
		// Load obj
		Resources::ResourcesManager::loadObj(m_filePath);

		setMeshes();
	}

	void Model::setMeshes()
	{
		if (m_mesh)
			return;

		std::vector<std::string>* modelChildrens = Resources::ResourcesManager::getMeshNames(m_filePath);

		if (!modelChildrens)
			return;

		// Get model childrens
		for (std::string& meshName : *modelChildrens)
		{
			Model child(m_transform, meshName);

			std::shared_ptr<Resources::Material> newMat = Resources::ResourcesManager::getMatByMeshName(meshName);

			if (newMat)
				child.m_material = newMat;
			else
				Core::Debug::Log::error("Cannot load the material from " + meshName);

			m_children.push_back(child);
		}
	}

	void Model::draw(std::shared_ptr<Resources::ShaderProgram> shaderProgram) const
	{
		if (m_mesh)
		{
			RenderManager::GLSetCapState(GL_CULL_FACE, hasFaceCulling);

			// Send model matrix to program
			shaderProgram->setUniform("model", m_transform->getGlobalModel().e, false, 1, 1);

			std::shared_ptr<Resources::Material> currentMat = m_material ? m_material : Resources::Material::defaultMaterial;

			// Send and bind material to program
			currentMat->sendToShader(shaderProgram);

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
			shaderProgram->setUniform("model", m_transform->getGlobalModel().e, false, 1, 1);

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
			shaderProgram->setUniform("model", modelCollider.e, false, 1, 1);
			shaderProgram->setUniform("color", color.e, true, 1, 1);

			// Draw the mesh
			m_mesh->draw();
		}

		// Draw children
		for (const Model& child : m_children)
			child.drawCollider(shaderProgram, modelCollider);
	}

	const std::string& Model::getPath() const
	{
		return m_filePath;
	}

	void Model::drawImGui()
	{
		if (ImGui::TreeNode(m_name.c_str()))
		{
			if (m_mesh)
			{
				if (m_material)
					m_material->drawImGui();

				ImGui::Checkbox("Has face culling", &hasFaceCulling);
			}

			for (Model& child : m_children)
				child.drawImGui();

			ImGui::TreePop();
		}
	}
}