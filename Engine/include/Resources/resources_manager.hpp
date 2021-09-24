#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include <atomic>

#include "singleton.hpp"

#include "thread_manager.hpp"

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
		std::atomic<bool> isLoading = false;
		std::chrono::system_clock::time_point loadStart;

		bool initialized = false;

		ResourcesManager();
		~ResourcesManager();
		
		std::atomic_flag lockPersistentResources = ATOMIC_FLAG_INIT;
		std::atomic_flag lockTextures = ATOMIC_FLAG_INIT;
		std::atomic_flag lockMeshes = ATOMIC_FLAG_INIT;
		std::atomic_flag lockMeshChildren = ATOMIC_FLAG_INIT;
		std::atomic_flag lockCubemaps = ATOMIC_FLAG_INIT;
		std::atomic_flag lockMaterials = ATOMIC_FLAG_INIT;

		ConcurrentQueue<Resource*> toInitInMainThread;

		std::unordered_map<std::string, std::vector<std::string>>		childrenMeshes;
		std::unordered_map<std::string, std::string>					childrenMaterials;

		std::vector<std::shared_ptr<Resource>>							persistentsResources;
		std::unordered_map<std::string, std::shared_ptr<Texture>>		textures;
		std::unordered_map<std::string, std::shared_ptr<CubeMap>>		cubeMaps;
		std::unordered_map<std::string, std::shared_ptr<Mesh>>			meshes;
		std::unordered_map<std::string, std::shared_ptr<Material>>		materials;
		std::unordered_map<std::string, std::shared_ptr<Font>>			fonts;

		std::unordered_map<std::string, std::shared_ptr<Shader>>		shaders;
		std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaderPrograms;

		std::unordered_map<std::string, std::shared_ptr<Recipe>>		recipes;

		void setDefaultResources();

		template <class C>
		void purgeMap(std::unordered_map<std::string, std::shared_ptr<C>>& map)
		{
			//std::erase_if(map, [](std::pair<const std::string&, std::shared_ptr<C>> &item) { return item->second.use_count() <= 1; });
			for (auto it = map.begin(); it != map.end();)
			{
				if (it->second.use_count() <= 1)
					it = map.erase(it);
				else
					it++;
			}
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

		static void checkLoadEnd();

		static std::shared_ptr<Font>	loadFont(const std::string& fontPath);
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

		static void drawImGui();

		template <class Fct, typename... Types>
		static void manageTask(Fct&& func, Types&&... args)
		{
			ResourcesManager* RM = instance();

			if (!RM->isLoading)
			{
				RM->loadStart = std::chrono::system_clock::now();
				RM->isLoading = true;
			}

			ThreadManager::manageTask("load", func, args...);
		}
	};
}