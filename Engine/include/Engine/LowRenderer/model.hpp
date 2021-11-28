#pragma once

#include <memory>
#include <deque>

#include "mesh.hpp"
#include "shader.hpp"
#include "material.hpp"

#define COLLIDER_COLOR Core::Maths::vec3(0.f, 1.f, 0.f)

namespace Physics
{
	class TransformComponent;
}

namespace LowRenderer
{
	class Model
	{
	private:
		std::shared_ptr<Resources::Mesh> m_mesh = nullptr;
		std::shared_ptr<Resources::Material> m_material = Resources::Material::defaultMaterial;

		std::vector<Model> m_children;
		std::string m_filePath;
		std::string m_name;

		Model(Physics::TransformComponent* transform, const std::string& meshName);

	public:
		bool hasFaceCulling = true;

		Physics::TransformComponent* m_transform = nullptr;

		Model(const std::string& filePath, Physics::TransformComponent* transform);

		Model() = default;

		void draw(std::shared_ptr<Resources::ShaderProgram> shaderProgram) const;
		void simpleDraw(std::shared_ptr<Resources::ShaderProgram> shaderProgram) const;
		void drawCollider(std::shared_ptr<Resources::ShaderProgram> shaderProgram, Core::Maths::mat4& modelCollider) const;
		void drawImGui();

		void loadMeshes();
		void setMeshes();

		const std::string& getPath() const;
	};
}
