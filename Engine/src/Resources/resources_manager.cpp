#include "resources_manager.hpp"

#include <fstream>

#include <imgui.h>

#include "debug.hpp"

#include "maths.hpp"
#include "utils.hpp"

namespace Resources
{
	ResourcesManager::ResourcesManager()
	{
		Core::Debug::Log::info("Creating the Resources Manager, the resources path will be " + resourcesPath);
	}

	ResourcesManager::~ResourcesManager()
	{
		Core::Debug::Log::info("Destroying the Resources Manager");
	}

	void ResourcesManager::setDefaultResources()
	{
		// Load the peristent textures
		std::shared_ptr<Texture> whiteTex = ResourcesManager::loadTexture("whiteTex", 1, 1, whiteBuffer, true);
		std::shared_ptr<Texture> blackTex = ResourcesManager::loadTexture("blackTex", 1, 1, blackBuffer, true);
		std::shared_ptr<Texture> noDiffuseTex = ResourcesManager::loadTexture("noDiffuseTex", 2, 2, noDiffuseBuffer, true);

		// Set the peristent textures
		Texture::defaultAlpha = whiteTex;
		Texture::defaultAmbient = whiteTex;
		Texture::defaultDiffuse = noDiffuseTex;
		Texture::defaultEmissive = blackTex;
		Texture::defaultSpecular = whiteTex;

		// Load the peristent material
		Material::defaultMaterial = ResourcesManager::loadMaterial("defaultMaterial_LERE", true);
	}

	void ResourcesManager::init(unsigned int workerCount)
	{
		ResourcesManager* RM = instance();

		// Check if the ResourcesManager instance is already initialized
		if (RM->initialized)
		{
			Core::Debug::Log::error("The Resources Manager is already initialized");
			return;
		}

		RM->initialized = true;
		Core::Debug::Log::info("Resources Manager initialized");

		Multithread::ThreadManager::init("load", workerCount);

		// Set the shader program
		loadShaderProgram("shader", "resources/shaders/vertexShader.vert", "resources/shaders/fragmentShader.frag", "", true);
		loadShaderProgram("skyBox", "resources/shaders/skyBox.vert", "resources/shaders/skyBox.frag", "", true);
		loadShaderProgram("colliderShader", "resources/shaders/vertexCollider.vert", "resources/shaders/fragmentCollider.frag", "", true);
		loadShaderProgram("spriteShader", "resources/shaders/spriteVertex.vert", "resources/shaders/spriteFragment.frag", "", true);
		loadShaderProgram("depthShader", "resources/shaders/depthShader.vert", "resources/shaders/depthShader.frag", "", true);
		loadShaderProgram("depthCubeShader", "resources/shaders/depthCubeShader.vert", "resources/shaders/depthShader.frag", "resources/shaders/depthCubeShader.geom", true);

		// Set the peristent obj
		loadObj("resources/obj/cube.obj", true);
		loadObj("resources/obj/sphere.obj", true);
		loadObj("resources/obj/plane.obj", true);
		loadObj("resources/obj/colliders/boxCollider.obj", true);
		loadObj("resources/obj/colliders/sphereCollider.obj", true);

		// Set default textures and materials
		RM->setDefaultResources();
	}

	void ResourcesManager::clearResources()
	{
		instance()->toInitInMainThread.clear();
	}

	void ResourcesManager::purgeResources()
	{
		ResourcesManager* RM = instance();

		RM->purgeMap(RM->materials, RM->lockMaterials);
		RM->purgeMap(RM->textures, RM->lockTextures);
		RM->purgeMap(RM->cubeMaps, RM->lockCubemaps);
		RM->purgeMap(RM->meshes, RM->lockMeshes);
		RM->purgeMap(RM->shaders);
		RM->purgeMap(RM->scripts);
		RM->purgeMap(RM->shaderPrograms);
 	}

	std::shared_ptr<Shader> ResourcesManager::loadShader(const std::string& shaderPath, bool setAsPersistent)
	{
		ResourcesManager* RM = instance();

		const auto& shaderIt = RM->shaders.find(shaderPath);

		// Check if the Shader is already loaded
		if (shaderIt != RM->shaders.end())
		{
			return shaderIt->second;
		}

		std::shared_ptr<Shader> shaderPtr = RM->shaders[shaderPath] = std::make_shared<Shader>(shaderPath);

		if (setAsPersistent)
			RM->persistentsResources.push_back(shaderPtr);

		return shaderPtr;
	}

	std::shared_ptr<ShaderProgram> ResourcesManager::loadShaderProgram(const std::string& programName, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath, bool setAsPersistent)
	{
		ResourcesManager* RM = instance();

		const auto& programIt = RM->shaderPrograms.find(programName);

		// Check if the ShaderProgram is already loaded
		if (programIt != RM->shaderPrograms.end())
		{
			return programIt->second;
		}

		std::shared_ptr programPtr = RM->shaderPrograms[programName] = std::make_shared<ShaderProgram>(programName, vertPath, fragPath, geomPath);

		if (setAsPersistent)
			RM->persistentsResources.push_back(programPtr);

		return programPtr;
	}

	void ResourcesManager::addToMainThreadInitializerQueue(Resource* resourcePtr)
	{
		ResourcesManager* RM = instance();

		// Add the resource to the initialization queue 
		RM->toInitInMainThread.tryPush(resourcePtr);
	}

	void ResourcesManager::mainThreadQueueInitialize()
	{
		ResourcesManager* RM = instance();

		// Initialize each resource that is in the queue
		while (!RM->toInitInMainThread.empty())
		{
			Resource* resource = nullptr;
			if (RM->toInitInMainThread.tryPop(resource))
				resource->mainThreadInitialization();
		}

		if (checkLoadEnd())
		{
			Resources::ResourcesManager::purgeResources();

			Core::Debug::Benchmarker::sceneLoadedCallback();
		}
	}

	bool ResourcesManager::checkLoadEnd()
	{
		ResourcesManager* RM = instance();

		if (!Multithread::ThreadManager::isEmpty("load") || !RM->isLoading || !RM->toInitInMainThread.empty())
			return false;

		RM->isLoading = false;

		Core::Debug::Benchmarker::stopChrono("load");

		auto totalDuration = Core::Debug::Benchmarker::getDuration("load");
		std::string totalDurationString = std::to_string(totalDuration.count() * 1000);

		if (Multithread::ThreadManager::isMonoThreaded())
		{
			Core::Debug::Log::info("The scene totally loaded in " + totalDurationString + " ms in mono-thread.");
			return true;
		}

		auto threadedLoadDuration = (Multithread::ThreadManager::getLastTime("load") - Core::Debug::Benchmarker::getTimerStart("load"));
		std::string threadedLoadDurationString = std::to_string(threadedLoadDuration.count() * 1000);

		std::string threadCountAsString = std::to_string(Multithread::ThreadManager::getWorkerCount("load"));
		Core::Debug::Log::info("The scene totally loaded in " + totalDurationString + " ms, without OpenGL initialization the scene took " + threadedLoadDurationString + " ms to load with " + threadCountAsString + " threads.");

		return true;
	}

	void ResourcesManager::reloadScripts()
	{
		ResourcesManager* RM = instance();

		Core::Debug::Log::info("Reloading scripts");

		for (auto& scriptPair : RM->scripts)
			scriptPair.second->killModule();

		RM->pyInstance.reload();

		for (auto& scriptPair : RM->scripts)
			scriptPair.second->initializeModule();
	}

	std::shared_ptr<Font> ResourcesManager::loadFont(const std::string& fontPath)
	{
		ResourcesManager* RM = instance();

		const auto& fontIt = RM->fonts.find(fontPath);

		// Check if the Texture is already loaded
		if (fontIt != RM->fonts.end())
			return fontIt->second;

		return RM->fonts[fontPath] = std::make_shared<Font>(fontPath);
	}

	std::shared_ptr<Script> ResourcesManager::loadScript(const std::string& scriptName)
	{
		ResourcesManager* RM = instance();

		while (RM->lockScripts.test_and_set());

		const auto& scriptIt = RM->scripts.find(scriptName);

		// Check if the Texture is already loaded
		if (scriptIt != RM->scripts.end())
		{
			RM->lockScripts.clear();
			return scriptIt->second;
		}

		std::shared_ptr<Script> script(new Script(scriptName));

		RM->scripts[scriptName] = script;

		RM->lockScripts.clear();

		script->initializeModule();

		return script;
	}

	std::shared_ptr<Texture> ResourcesManager::loadTexture(const std::string& texturePath, bool setAsPersistent)
	{
		ResourcesManager* RM = instance();

		while (RM->lockTextures.test_and_set());

		const auto& textureIt = RM->textures.find(texturePath);

		// Check if the Texture is already loaded
		if (textureIt != RM->textures.end())
		{
			RM->lockTextures.clear();
			return textureIt->second;
		}

		// Create the texture if it does not exist
		std::shared_ptr<Texture> texturePtr(new Texture(texturePath));

		if (setAsPersistent)
		{
			while (RM->lockPersistentResources.test_and_set());

			// If the texture is persistent, add it to the persistent resources vector
			RM->persistentsResources.push_back(texturePtr);

			RM->lockPersistentResources.clear();
		}

		RM->textures[texturePath] = texturePtr;

		RM->lockTextures.clear();

		// Generate its buffer with the threading system
		manageTask(&Texture::generateBuffer, texturePtr.get());

		return texturePtr;
	}

	std::shared_ptr<Texture> ResourcesManager::loadTexture(const std::string& name, int width, int height, float* data, bool setAsPersistent)
	{
		ResourcesManager* RM = instance();

		while (RM->lockTextures.test_and_set());

		const auto& textureIt = RM->textures.find(name);

		// Check if the Texture is already loaded
		if (textureIt != RM->textures.end())
		{
			RM->lockTextures.clear();
			return textureIt->second;
		}
		
		// If the texture is persistent, add it to the persistent resources vector
		std::shared_ptr<Texture> texturePtr(new Texture(name, width, height, data));

		if (setAsPersistent)
		{
			while (RM->lockPersistentResources.test_and_set());

			RM->persistentsResources.push_back(texturePtr);

			RM->lockPersistentResources.clear();
		}

		RM->textures[name] = texturePtr;

		RM->lockTextures.clear();

		return texturePtr;
	}

	std::shared_ptr<CubeMap> ResourcesManager::loadCubeMap(const std::vector<std::string>& cubeMapPaths, bool setAsPersistent)
	{
		if (cubeMapPaths.size() == 0)
			return nullptr;

		ResourcesManager* RM = instance();

		while (!RM->lockCubemaps.test_and_set());

		std::string pathsDir = Utils::getDirectory(cubeMapPaths.back());
		const auto& cubeMapIt = RM->cubeMaps.find(pathsDir);

		// Check if the Texture is already loaded
		if (cubeMapIt != RM->cubeMaps.end())
		{
			RM->lockCubemaps.clear();
			return cubeMapIt->second;
		}

		// If the cubemap is persistent, add it to the persistent resources vector
		std::shared_ptr<CubeMap> cubeMapPtr(new CubeMap(cubeMapPaths));

		if (setAsPersistent)
		{
			while (RM->lockPersistentResources.test_and_set());

			RM->persistentsResources.push_back(cubeMapPtr);

			RM->lockPersistentResources.clear();
		}

		RM->cubeMaps[pathsDir] = cubeMapPtr;

		RM->lockCubemaps.clear();

		// Generate its buffers with the threading system
		manageTask(&CubeMap::generateBuffers, cubeMapPtr.get());

		return cubeMapPtr;
	}

	std::shared_ptr<Material> ResourcesManager::loadMaterial(const std::string& materialPath, bool setAsPersistent)
	{
		ResourcesManager* RM = instance();

		const auto& materialIt = RM->materials.find(materialPath);

		// Check if the Material is already loaded
		if (materialIt != RM->materials.end())
		{
			return materialIt->second;
		}

		// If the material is persistent, add it to the persistent resources vector
		std::shared_ptr<Material> matPtr = RM->materials[materialPath] = std::make_shared<Material>(materialPath);

		if (setAsPersistent)
		{
			while (RM->lockPersistentResources.test_and_set());

			// Generate its buffers with the threading system
			RM->persistentsResources.push_back(matPtr);

			RM->lockPersistentResources.clear();
		}

		return matPtr;
	}

	std::shared_ptr<Recipe> ResourcesManager::loadRecipe(const std::string& recipePath, bool setAsPersistent)
	{
		ResourcesManager* RM = instance();

		const auto& recipeIt = RM->recipes.find(recipePath);

		// Check if the Material is already loaded
		if (recipeIt != RM->recipes.end())
		{
			return recipeIt->second;
		}

		std::shared_ptr<Recipe> recipePtr = RM->recipes[recipePath] = std::make_shared<Recipe>(recipePath);

		if (setAsPersistent)
		{
			while (RM->lockPersistentResources.test_and_set());

			RM->persistentsResources.push_back(recipePtr);

			RM->lockPersistentResources.clear();
		}

		return recipePtr;
	}

	// Load an obj with mtl (do triangulation)
	void ResourcesManager::loadObj(std::string filePath, bool setAsPersistent)
	{
		std::string correctPath = getResourcesPath() + filePath;

		std::ifstream dataObj(correctPath.c_str());

		// Check if the file exists
		if (!dataObj)
		{
			Core::Debug::Log::error("Unable to read the file : " + filePath);
			dataObj.close();
			return;
		}

		ResourcesManager* RM = instance();

		while (RM->lockMeshChildren.test_and_set());

		// Check if the object is already loaded
		if (RM->childrenMeshes.find(filePath) != RM->childrenMeshes.end())
		{
			Core::Debug::Log::info("Model at " + filePath + " is already loaded");
			RM->lockMeshChildren.clear();
			return;
		}

		RM->lockMeshChildren.clear();

		Core::Debug::Log::info("Start loading obj " + filePath);

		std::string dirPath = Utils::getDirectory(filePath);

		Core::Debug::Log::info("Loading meshes");

		std::string meshName = filePath;
		std::string meshSubString;

		// Set the mesh ptr to put the ptr in
		std::shared_ptr<Mesh> meshPtr;

		std::array<unsigned int, 3> countArray{ 0u, 0u, 0u };
		std::array<unsigned int, 3> lastCountArray{ 0u, 0u, 0u };

		for (std::string line; std::getline(dataObj, line);)
		{
			meshSubString += line + '\n';

			std::istringstream iss(line);
			std::string type;

			iss >> type;

			if (type == "#" || type == "" || type == "\n")
				continue;

			if (type == "o")
			{
				iss >> meshName;

				if (meshPtr)
				{
					// Parse the current mesh with the substring and the offsets
					manageTask(&Mesh::parse, meshPtr.get(), meshSubString, lastCountArray);
					meshPtr = nullptr;
				}

				lastCountArray = countArray;
				meshSubString.clear();

				while (RM->lockMeshes.test_and_set());

				// Compute and add the mesh
				if (RM->meshes.find(meshName) == RM->meshes.end())
				{
					meshPtr = RM->meshes[meshName] = std::make_shared<Mesh>(meshName, filePath);

					if (setAsPersistent)
					{
						while (RM->lockPersistentResources.test_and_set());

						// If the mesh is persistent, add it to the persistent resources vector
						RM->persistentsResources.push_back(meshPtr);

						RM->lockPersistentResources.clear();
					}
				}

				RM->lockMeshes.clear();

				while (RM->lockMeshChildren.test_and_set());

				// Set the dependency with the meshes
				RM->childrenMeshes[filePath].push_back(meshName);

				RM->lockMeshChildren.clear();
			}
			// Count the attributs offsets for the mesh parsing
			else if (type == "v")
				countArray[0]++;
			else if (type == "vt")
				countArray[1]++;
			else if (type == "vn")
				countArray[2]++;
			else if (type == "usemtl")
			{
				std::string matName;
				iss >> matName;

				RM->childrenMaterials[meshName] = matName;

				while (RM->lockMaterials.test_and_set());

				// Create an empty material if it does not exist
				if (RM->materials.find(matName) == RM->materials.end())
					RM->materials[matName] = std::make_shared<Material>(matName);

				RM->lockMaterials.clear();
			}
			else if (type == "mtllib")
			{
				std::string mtlName;
				iss >> mtlName;

				// Load mtl file
				manageTask(&ResourcesManager::loadMaterials, dirPath, mtlName);
			}
		}

		if (!meshPtr)
		{
			while (RM->lockMeshes.test_and_set());

			// Compute and add the mesh
			if (RM->meshes.find(meshName) == RM->meshes.end())
			{
				meshPtr = RM->meshes[meshName] = std::make_shared<Mesh>(meshName, filePath);

				if (setAsPersistent)
				{
					while (RM->lockPersistentResources.test_and_set());

					// If the mesh is persistent, add it to the persistent resources vector
					RM->persistentsResources.push_back(meshPtr);

					RM->lockPersistentResources.clear();
				}
			}

			RM->lockMeshes.clear();

			while (RM->lockMeshChildren.test_and_set());

			// Set the dependency with the meshes
			RM->childrenMeshes[filePath].push_back(meshName);

			RM->lockMeshChildren.clear();
		}

		if (meshPtr)
			manageTask(&Mesh::parse, meshPtr.get(), meshSubString, lastCountArray);

		Core::Debug::Log::info("Finish loading obj " + filePath);
	}

	std::vector<std::string>* ResourcesManager::getMeshNames(const std::string& filePath)
	{
		ResourcesManager* RM = instance();

		auto meshNameIt = RM->childrenMeshes.find(filePath);

		// Check if meshes are linked to the filePath
		if (meshNameIt == RM->childrenMeshes.end())
		{
			Core::Debug::Log::error("Can not find mesh children at " + filePath);
			return nullptr;
		}

		return &meshNameIt->second;
	}

	std::shared_ptr<Mesh> ResourcesManager::getMeshByName(const std::string& meshName)
	{
		ResourcesManager* RM = instance();

		auto meshIt = RM->meshes.find(meshName);

		// Check if the mesh exist
		if (meshIt == RM->meshes.end())
		{
			Core::Debug::Log::error("Can not find mesh named " + meshName);
			return nullptr;
		}

		return meshIt->second;
	}

	std::shared_ptr<Material> ResourcesManager::getMatByMeshName(const std::string& meshName)
	{
		ResourcesManager* RM = instance();

		auto materialIt = RM->childrenMaterials.find(meshName);

		// Check if a material is link to the mesh name
		if (materialIt == RM->childrenMaterials.end())
		{
			Core::Debug::Log::error("Can not find material at " + meshName);
			return nullptr;
		}

		// Load and return the material
		return ResourcesManager::loadMaterial(materialIt->second);
	}

	void ResourcesManager::loadMaterials(const std::string& dirPath, const std::string& mtlName)
	{
		std::string filePath = getResourcesPath() + dirPath + mtlName;

		// Check if the file exist
		std::ifstream dataMat(filePath.c_str());
		if (!dataMat)
		{
			Core::Debug::Log::error("Unable to read the file: " + filePath);
			dataMat.close();
			return;
		}

		ResourcesManager* RM = instance();

		std::string matName;

		Core::Debug::Log::info("Loading materials at " + filePath);

		std::string matSubString;

		// Create an empty ptr for the next material
		std::shared_ptr<Material> matPtr;

		// Get all mesh materials
		std::string line;
		for (std::string line; std::getline(dataMat, line);)
		{
			matSubString += line + '\n';

			std::istringstream iss(line);
			std::string type;
			iss >> type;

			if (type != "newmtl")
				continue;

			if (matPtr)
			{
				// Parse the current material with the substring
				manageTask(&Material::parse, matPtr.get(), matSubString, dirPath);
				matPtr = nullptr;
			}

			matSubString.clear();

			iss >> matName;

			while (RM->lockMaterials.test_and_set());

			auto matIt = RM->materials.find(matName);

			// Check if the material is already loaded
			if (matIt == RM->materials.end())
				matPtr = RM->materials[matName] = std::make_shared<Material>(matName);
			else
				matPtr = matIt->second;

			RM->lockMaterials.clear();
		}

		// Parse the current material with the substring
		if (matPtr)
			manageTask(&Material::parse, matPtr.get(), matSubString, dirPath);
	}

	std::string ResourcesManager::getResourcesPath()
	{
		return instance()->resourcesPath + '/';
	}

	void ResourcesManager::drawImGui()
	{
		ResourcesManager* RM = instance();

		if (ImGui::Begin("Resources Manager"))
		{
			if (ImGui::Button("Reload scripts"))
				reloadScripts();

			if (ImGui::CollapsingHeader("Textures:"))
			{
				for (auto& texturePtr : RM->textures)
					texturePtr.second->drawImGui();
			}

			if (ImGui::CollapsingHeader("Materials:"))
			{
				for (auto& materialPtr : RM->materials)
					materialPtr.second->drawImGui();
			}
		}
		ImGui::End();
	}
}