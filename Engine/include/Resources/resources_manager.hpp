#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "singleton.hpp"

#include "cube_map.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "scene.hpp"
#include "mesh.hpp"

namespace Resources
{
	class ResourcesManager final : public Singleton<ResourcesManager>
	{
		friend Singleton<ResourcesManager>;

	private:
		bool initialized = false;

		ResourcesManager();
		~ResourcesManager();

		std::unordered_map<std::string, std::vector<std::string>>		childrenMeshes;
		std::unordered_map<std::string, std::string>		childrenMaterials;

		std::unordered_map<std::string, std::shared_ptr<Texture>>		textures;
		std::unordered_map<std::string, std::shared_ptr<CubeMap>>		cubeMaps;
		std::unordered_map<std::string, std::shared_ptr<Mesh>>			meshes;
		std::unordered_map<std::string, std::shared_ptr<Material>>		materials;

		std::unordered_map<std::string, std::shared_ptr<Shader>>		shaders;
		std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaderPrograms;

		std::unordered_map<std::string, std::shared_ptr<Scene>>			scenes;

		void setDefaultResources();


	public:
		static void init();

		static void loadObj(const std::string& filePath);
		static void loadMaterialsFromMtl(const std::string& dirPath, const std::string& fileName);

		static std::shared_ptr<Texture> loadTexture(const std::string& texturePath);
		static std::shared_ptr<Texture> loadTexture(const std::string& name, int width, int height, float* data);
		static std::shared_ptr<CubeMap> loadCubeMap(const std::vector<std::string>& cubeMapPaths);
		static std::shared_ptr<Material> loadMaterial(const std::string& materialPath);
		static std::shared_ptr<Scene> loadScene(const std::string& scenePath);
		static std::shared_ptr<Shader> loadShader(const std::string& shaderPath);
		static std::shared_ptr<ShaderProgram> loadShaderProgram(const std::string& programName, const std::string& vertPath = "", const std::string& fragPath = "");

		static std::vector<std::string>* getMeshNames(const std::string& filePath);
		static std::shared_ptr<Mesh> getMeshByName(const std::string& meshName);
		static std::shared_ptr<Material> getMatByMeshName(const std::string& meshName);
	};
}