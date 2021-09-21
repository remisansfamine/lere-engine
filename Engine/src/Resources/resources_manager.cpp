#include "resources_manager.hpp"

#include <fstream>

#include "debug.hpp"

#include "maths.hpp"
#include "utils.hpp"

#include "thread_pool.hpp"

#include "imgui.h"

namespace Resources
{
	// White color
	float whiteBuffer[4] = { 1.f, 1.f, 1.f, 1.f };

	// Black color
	float blackBuffer[4] = { 0.f, 0.f, 0.f, 0.f };

	// Purple and black grid
	float noDiffuseBuffer[16] = { 1.f, 0.f, 0.863f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.863f, 1.f };

	ResourcesManager::ResourcesManager()
	{
		Core::Debug::Log::info("Creating the Resources Manager");
	}

	ResourcesManager::~ResourcesManager()
	{
		Core::Debug::Log::info("Destroying the Resources Manager");
	}

	void ResourcesManager::setDefaultResources()
	{
		// Load the default textures
		std::shared_ptr<Texture> whiteTex = ResourcesManager::getTexture("whiteTex", 1, 1, whiteBuffer);
		std::shared_ptr<Texture> blackTex = ResourcesManager::getTexture("blackTex", 1, 1, blackBuffer);
		std::shared_ptr<Texture> noDiffuseTex = ResourcesManager::getTexture("noDiffuseTex", 2, 2, noDiffuseBuffer);

		// Set the default textures
		Texture::defaultAlpha = whiteTex;
		Texture::defaultAmbient = whiteTex;
		Texture::defaultDiffuse = noDiffuseTex;
		Texture::defaultEmissive = blackTex;
		Texture::defaultSpecular = whiteTex;

		// Load the default material
		Material::defaultMaterial = ResourcesManager::getMaterial("defaultMaterial_LERE");
	}

	void ResourcesManager::init()
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

		loadShaderProgram("shader", "resources/shaders/vertexShader.vert", "resources/shaders/fragmentShader.frag");
		loadShaderProgram("skyBox", "resources/shaders/skyBox.vert", "resources/shaders/skyBox.frag");
		loadShaderProgram("colliderShader", "resources/shaders/vertexCollider.vert", "resources/shaders/fragmentCollider.frag");
		loadShaderProgram("spriteShader", "resources/shaders/spriteVertex.vert", "resources/shaders/spriteFragment.frag");
		loadShaderProgram("depthShader", "resources/shaders/depthShader.vert", "resources/shaders/depthShader.frag");
		loadShaderProgram("depthCubeShader", "resources/shaders/depthCubeShader.vert", "resources/shaders/depthShader.frag", "resources/shaders/depthCubeShader.geom");

		loadObj("resources/obj/cube.obj");
		loadObj("resources/obj/sphere.obj");
		loadObj("resources/obj/plane.obj");
		loadObj("resources/obj/colliders/boxCollider.obj");
		loadObj("resources/obj/colliders/sphereCollider.obj");

		// Set default textures and materials
		RM->setDefaultResources();
	}

	void ResourcesManager::clearResources()
	{
		ResourcesManager* RM = instance();

		RM->clearMap(RM->textures);
		RM->clearMap(RM->cubeMaps);
		RM->clearMap(RM->meshes);
		RM->clearMap(RM->materials);
		//RM->clearMap(RM->shaders);
		//RM->clearMap(RM->shaderPrograms);
	}

	std::shared_ptr<Font> ResourcesManager::getFont(const std::string& fontPath)
	{
		return loadFont(fontPath);
	}

	std::shared_ptr<Texture> ResourcesManager::getTexture(const std::string& texturePath)
	{
		return loadTexture(texturePath);
	}

	std::shared_ptr<Texture> ResourcesManager::getTexture(const std::string& name, int width, int height, float* data)
	{
		return loadTexture(name, width, height, data);
	}

	std::shared_ptr<CubeMap> ResourcesManager::getCubeMap(const std::vector<std::string>& cubeMapPaths)
	{
		ResourcesManager* RM = instance();

		std::string pathsDir = Utils::getDirectory(cubeMapPaths.back());

		return loadCubeMap(cubeMapPaths);
	}

	std::shared_ptr<Material> ResourcesManager::getMaterial(const std::string& materialPath)
	{
		return loadMaterial(materialPath);
	}

	std::shared_ptr<Recipe> ResourcesManager::getRecipe(const std::string& recipePath)
	{
		return loadRecipe(recipePath);
	}

	std::shared_ptr<Shader> ResourcesManager::getShader(const std::string& shaderPath)
	{
		return loadShader(shaderPath);
	}

	std::shared_ptr<ShaderProgram> ResourcesManager::getShaderProgram(const std::string& programName, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath)
	{
		return loadShaderProgram(programName);
	}

	std::shared_ptr<Shader> ResourcesManager::loadShader(const std::string& shaderPath)
	{
		ResourcesManager* RM = instance();

		const auto& shaderIt = RM->shaders.find(shaderPath);

		// Check if the Shader is already loaded
		if (shaderIt != RM->shaders.end())
		{
			return shaderIt->second;
		}

		return RM->shaders[shaderPath] = std::make_shared<Shader>(shaderPath);
	}

	std::shared_ptr<ShaderProgram> ResourcesManager::loadShaderProgram(const std::string& programName, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath)
	{
		ResourcesManager* RM = instance();

		const auto& programIt = RM->shaderPrograms.find(programName);

		// Check if the ShaderProgram is already loaded
		if (programIt != RM->shaderPrograms.end())
		{
			return programIt->second;
		}

		return RM->shaderPrograms[programName] = std::make_shared<ShaderProgram>(programName, vertPath, fragPath, geomPath);
	}

	void ResourcesManager::addToMainThreadInitializerQueue(Resource* resourcePtr)
	{
		instance()->toInitInMainThread.tryPush(resourcePtr);
	}

	void ResourcesManager::mainThreadQueueInitialize()
	{
		ResourcesManager* RM = instance();

		while (!RM->toInitInMainThread.empty())
		{
			Resource* resource = nullptr;

			RM->toInitInMainThread.tryPop(resource);

			if (resource)
				resource->mainThreadInitialization();
		}
	}

	std::shared_ptr<Font> ResourcesManager::loadFont(const std::string& fontPath)
	{
		ResourcesManager* RM = instance();

		const auto& fontIt = RM->fonts.find(fontPath);

		// Check if the Texture is already loaded
		if (fontIt != RM->fonts.end())
		{
			return fontIt->second;
		}

		return RM->fonts[fontPath] = std::make_shared<Font>(fontPath);
	}

	std::shared_ptr<Texture> ResourcesManager::loadTexture(const std::string& texturePath)
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

		std::shared_ptr<Texture> texturePtr(new Texture(texturePath));

		RM->textures[texturePath] = texturePtr;

		RM->lockTextures.clear();

		ThreadPool::addTask(std::bind(&Texture::generateBuffer, texturePtr));
		//texturePtr->generateBuffer();

		return texturePtr;
	}

	std::shared_ptr<Texture> ResourcesManager::loadTexture(const std::string& name, int width, int height, float* data)
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

		std::shared_ptr<Texture> texturePtr(new Texture(name, width, height, data));

		RM->textures[name] = texturePtr;

		RM->lockTextures.clear();

		return texturePtr;
	}

	std::shared_ptr<CubeMap> ResourcesManager::loadCubeMap(const std::vector<std::string>& cubeMapPaths)
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

		std::shared_ptr<CubeMap> cubeMapPtr(new CubeMap(cubeMapPaths));

		RM->cubeMaps[pathsDir] = cubeMapPtr;

		RM->lockCubemaps.clear();

		ThreadPool::addTask(std::bind(&CubeMap::generateBuffers, RM->cubeMaps[pathsDir]));
		//RM->cubeMaps[pathsDir]->generateBuffers();

		return cubeMapPtr;
	}

	std::shared_ptr<Material> ResourcesManager::loadMaterial(const std::string& materialPath)
	{
		ResourcesManager* RM = instance();

		const auto& materialIt = RM->materials.find(materialPath);

		// Check if the Material is already loaded
		if (materialIt != RM->materials.end())
		{
			return materialIt->second;
		}

		return RM->materials[materialPath] = std::make_shared<Material>(materialPath);
	}

	std::shared_ptr<Recipe> ResourcesManager::loadRecipe(const std::string& recipePath)
	{
		ResourcesManager* RM = instance();

		const auto& recipeIt = RM->recipes.find(recipePath);

		// Check if the Material is already loaded
		if (recipeIt != RM->recipes.end())
		{
			return recipeIt->second;
		}

		return RM->recipes[recipePath] = std::make_shared<Recipe>(recipePath);
	}

	// Load an obj with mtl (do triangulation)
	void ResourcesManager::loadObj(std::string filePath)

	{
		std::ifstream dataObj(filePath.c_str());

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

		bool isFirstObject = true;

		Core::Debug::Log::info("Loading meshes");

		std::string meshName = filePath;
		std::string meshSubString;

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

				if (isFirstObject)
				{
					isFirstObject = false;
					meshSubString.clear();
					lastCountArray = countArray;
				}
				else
				{
					if (meshPtr)
					{
						ThreadPool::addTask(std::bind(&Mesh::parse, meshPtr, meshSubString, lastCountArray));
						meshPtr = nullptr;

						lastCountArray = countArray;
						meshSubString.clear();
					}
				}

				while (RM->lockMeshes.test_and_set());

				// Compute and add the mesh
				if (RM->meshes.find(meshName) == RM->meshes.end())
				{
					meshPtr = RM->meshes[meshName] = std::make_shared<Mesh>();
					meshPtr->m_name = meshName;
				}

				RM->lockMeshes.clear();

				while (RM->lockMeshChildren.test_and_set());

				RM->childrenMeshes[filePath].push_back(meshName);

				RM->lockMeshChildren.clear();
			}
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
			}
			else if (type == "mtllib")
			{
				std::string mtlName;
				iss >> mtlName;

				// Load mtl file
				//ThreadPool::addTask(std::bind(&ResourcesManager::loadMaterials, dirPath, mtlName));
				loadMaterials(dirPath, mtlName);
			}
		}

		if (isFirstObject)
		{
			while (RM->lockMeshes.test_and_set());

			// Compute and add the mesh
			if (RM->meshes.find(meshName) == RM->meshes.end())
			{
				meshPtr = RM->meshes[meshName] = std::make_shared<Mesh>();
				meshPtr->m_name = meshName;
			}

			RM->lockMeshes.clear();

			while (RM->lockMeshChildren.test_and_set());

			RM->childrenMeshes[filePath].push_back(meshName);

			RM->lockMeshChildren.clear();
		}

		if (meshPtr)
			ThreadPool::addTask(std::bind(&Mesh::parse, meshPtr, meshSubString, lastCountArray));

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
		return ResourcesManager::getMaterial(materialIt->second);
	}

	void ResourcesManager::loadMaterials(const std::string& dirPath, const std::string& mtlName)
	{
		std::string filePath = dirPath + mtlName;

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
		bool isFirstMat = true;

		Core::Debug::Log::info("Loading materials at " + filePath);

		std::string matSubString;

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

			if (isFirstMat)
				isFirstMat = false;
			else
			{
				std::shared_ptr<Material> matPtr;

				while (RM->lockMaterials.test_and_set());

				// Check if the material is already loaded
				if (RM->materials.find(matName) == RM->materials.end())
					matPtr = RM->materials[matName] = std::make_shared<Material>(matName);

				RM->lockMaterials.clear();

				// Add the material
				if (matPtr)
					ThreadPool::addTask(std::bind(&Material::parse, matPtr, matSubString, dirPath));
			}

			iss >> matName;

			matSubString.clear();
		}

		std::shared_ptr<Material> matPtr;

		while (RM->lockMaterials.test_and_set());

		// Check if the material is already loaded
		if (RM->materials.find(matName) == RM->materials.end())
			matPtr = RM->materials[matName] = std::make_shared<Material>(matName);

		RM->lockMaterials.clear();

		// Add the material
		if (matPtr)
			ThreadPool::addTask(std::bind(&Material::parse, matPtr, matSubString, dirPath));
	}

	void ResourcesManager::drawImGui()
	{
		ResourcesManager* RM = instance();

		if (ImGui::Begin("Resources Manager"))
		{
			ImGui::Checkbox("Is load mono-threaded", &RM->monoThread);

			if (ImGui::CollapsingHeader("Textures:"))
			{
				for (auto& texturePtr : RM->textures)
				{
					if (ImGui::CollapsingHeader(texturePtr.first.c_str()))
						texturePtr.second->drawImGui();
				}
			}
		}
		ImGui::End();
	}
}