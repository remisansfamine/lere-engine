#pragma once

#include <unordered_map>
#include <filesystem>
#include <string>
#include <memory>
#include <atomic>

#include "singleton.hpp"

#include "thread_manager.hpp"
#include "benchmarker.hpp"

#include "script.hpp"
#include "character.hpp"
#include "cube_map.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "recipe.hpp"
#include "scene.hpp"
#include "mesh.hpp"

#include "concurrent_queue.hpp"

namespace Resources
{
	class ResourcesManager final : public Singleton<ResourcesManager>
	{
		friend Singleton<ResourcesManager>;

	private:
		// White color
		float whiteBuffer[4] = { 1.f, 1.f, 1.f, 1.f };

		// Black color
		float blackBuffer[4] = { 0.f, 0.f, 0.f, 0.f };

		// Purple and black grid
		float noDiffuseBuffer[16] = { 1.f, 0.f, 0.863f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.863f, 1.f };

		CPyInstance pyInstance;

		std::atomic<bool> isLoading = false;

		bool initialized = false;

		ResourcesManager();
		~ResourcesManager();
		
		std::atomic_flag stbiFlag = ATOMIC_FLAG_INIT;

		std::atomic_flag lockPersistentResources = ATOMIC_FLAG_INIT;
		std::atomic_flag lockTextures = ATOMIC_FLAG_INIT;
		std::atomic_flag lockScripts = ATOMIC_FLAG_INIT;
		std::atomic_flag lockMeshes = ATOMIC_FLAG_INIT;
		std::atomic_flag lockMeshChildren = ATOMIC_FLAG_INIT;
		std::atomic_flag lockCubemaps = ATOMIC_FLAG_INIT;
		std::atomic_flag lockMaterials = ATOMIC_FLAG_INIT;

		ConcurrentQueue<Resource*> toInitInMainThread;

		std::unordered_map<std::string, std::vector<std::string>>		childrenMeshes;
		std::unordered_map<std::string, std::string>					childrenMaterials;

		std::vector<std::shared_ptr<Resource>>							persistentsResources;
		std::unordered_map<std::string, std::shared_ptr<Texture>>		textures;
		std::unordered_map<std::string, std::shared_ptr<Script>>		scripts;
		std::unordered_map<std::string, std::shared_ptr<CubeMap>>		cubeMaps;
		std::unordered_map<std::string, std::shared_ptr<Mesh>>			meshes;
		std::unordered_map<std::string, std::shared_ptr<Material>>		materials;
		std::unordered_map<std::string, std::shared_ptr<Font>>			fonts;

		std::unordered_map<std::string, std::shared_ptr<Shader>>		shaders;
		std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaderPrograms;

		std::unordered_map<std::string, std::shared_ptr<Recipe>>		recipes;

		std::string resourcesPath = std::filesystem::current_path().string();

		void setDefaultResources();

		template <class C>
		void purgeCallback(const std::shared_ptr<C>& resourcePtr) { }

		template <>
		void purgeCallback(const std::shared_ptr<Mesh>& meshPtr)
		{
			// Remove the dependency with the meshes and the materials
			childrenMeshes.erase(meshPtr->parentMeshName);
			childrenMaterials.erase(meshPtr->m_name);
		}

		template <class C>
		void purgeMap(std::unordered_map<std::string, std::shared_ptr<C>>& map)
		{
			// Remove each resources that is not used
			std::erase_if(map, [this](const std::pair<const std::string&, const std::shared_ptr<C>&> &pair) {
				if (pair.second.use_count() > 1)
					return false;

 				purgeCallback(pair.second);
				return true;
				});
		}

		template <class C>
		void purgeMap(std::unordered_map<std::string, std::shared_ptr<C>>& map, std::atomic_flag& mapFlag)
		{
			while (mapFlag.test_and_set());
			purgeMap(map);
			mapFlag.clear();
		}

	public:
		static void init(unsigned int workerCount);

		static void loadObj(std::string filePath, bool setAsPersistent = false);
		static void loadMaterials(const std::string& dirPath, const std::string& mtlName);

		static void clearResources();
		static void purgeResources();

		static void addToMainThreadInitializerQueue(Resource* resourcePtr);

		static void mainThreadQueueInitialize();

		static bool checkLoadEnd();

		static void reloadScripts();

		static std::shared_ptr<Font>	loadFont(const std::string& fontPath);
		static std::shared_ptr<Script> loadScript(const std::string& scriptName);
		static std::shared_ptr<Texture> loadTexture(const std::string& texturePath, bool setAsPersistent = false);
		static std::shared_ptr<Texture> loadTexture(const std::string& name, int width, int height, float* data, bool setAsPersistent = false);
		static std::shared_ptr<CubeMap> loadCubeMap(const std::vector<std::string>& cubeMapPaths, bool setAsPersistent = false);
		static std::shared_ptr<Material> loadMaterial(const std::string& materialPath, bool setAsPersistent = false);
		static std::shared_ptr<Recipe> loadRecipe(const std::string& recipePath, bool setAsPersistent = false);
		static std::shared_ptr<Shader> loadShader(const std::string& shaderPath, bool setAsPersistent = false);
		static std::shared_ptr<ShaderProgram> loadShaderProgram(const std::string& programName, const std::string& vertPath = "", const std::string& fragPath = "", const std::string& geomPath = "", bool setAsPersistent = false);

		static std::vector<std::string>* getMeshNames(const std::string& filePath);
		static std::shared_ptr<Mesh> getMeshByName(const std::string& meshName);
		static std::shared_ptr<Material> getMatByMeshName(const std::string& meshName);

		static std::string getResourcesPath();

		static void drawImGui();

		template <class Fct, typename... Types>
		static void manageTask(Fct&& func, Types&&... args)
		{
			ResourcesManager* RM = instance();

			// Set the chronos
			if (!RM->isLoading)
			{
				Core::Debug::Benchmarker::startChrono("load");
				Core::Debug::Benchmarker::startChrono("loadWithOpenGL");
				RM->isLoading = true;
			}

			// Add the task to the thread manager
			Multithread::ThreadManager::manageTask("load", func, args...);
		}
	};
}