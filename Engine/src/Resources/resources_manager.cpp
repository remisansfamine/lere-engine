#include "resources_manager.hpp"

#include <fstream>
#include <sstream>

#include "debug.hpp"

#include "maths.hpp"
#include "utils.hpp"

namespace Resources
{
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
		// White color
		float whiteBuffer[4] = { 1.f, 1.f, 1.f, 1.f };

		// Black color
		float blackBuffer[4] = { 0.f, 0.f, 0.f, 0.f };

		// Purple and black grid
		//float noDiffuseBuffer[16] = { 1.f, 0.f, 0.863f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.863f, 1.f };
		float noDiffuseBuffer[4] = { 1.f, 1.f, 1.f, 1.f };

		// Load the default textures
		std::shared_ptr<Texture> whiteTex = ResourcesManager::loadTexture("whiteTex", 1, 1, whiteBuffer);
		std::shared_ptr<Texture> blackTex = ResourcesManager::loadTexture("blackTex", 1, 1, blackBuffer);
		std::shared_ptr<Texture> noDiffuseTex = ResourcesManager::loadTexture("noDiffuseTex", 1, 1, noDiffuseBuffer);

		// Set the default textures
		Texture::defaultAlpha = whiteTex;
		Texture::defaultAmbient = whiteTex;
		Texture::defaultDiffuse = noDiffuseTex;
		Texture::defaultEmissive = blackTex;
		Texture::defaultSpecular = whiteTex;

		// Load the default material
		Material::defaultMaterial = ResourcesManager::loadMaterial("defaultMaterial_LERE");
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

		std::shared_ptr<float> test;
	}

	void ResourcesManager::clearResources()
	{
		ResourcesManager* RM = instance();

		RM->clearMap(RM->textures);
		RM->clearMap(RM->cubeMaps);
		RM->clearMap(RM->meshes);
		RM->clearMap(RM->materials);
		RM->clearMap(RM->shaders);
		//RM->clearMap(RM->shaderPrograms);
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
			return programIt->second;

		return RM->shaderPrograms[programName] = std::make_shared<ShaderProgram>(programName, vertPath, fragPath, geomPath);
	}

	std::shared_ptr<Texture> ResourcesManager::loadTexture(const std::string& texturePath)
	{
		ResourcesManager* RM = instance();

		const auto& textureIt = RM->textures.find(texturePath);

		// Check if the Texture is already loaded
		if (textureIt != RM->textures.end())
		{
			return textureIt->second;
		}

		return RM->textures[texturePath] = std::make_shared<Texture>(texturePath);
	}

	std::shared_ptr<Texture> ResourcesManager::loadTexture(const std::string& name, int width, int height, float* data)
	{
		ResourcesManager* RM = instance();

		const auto& textureIt = RM->textures.find(name);

		// Check if the Texture is already loaded
		if (textureIt != RM->textures.end())
		{
			return textureIt->second;
		}

		return RM->textures[name] = std::make_shared<Texture>(width, height, data);
	}

	std::shared_ptr<CubeMap> ResourcesManager::loadCubeMap(const std::vector<std::string>& cubeMapPaths)
	{
		ResourcesManager* RM = instance();

		if (cubeMapPaths.size() == 0)
			return nullptr;

		std::string pathsDir = Utils::getDirectory(cubeMapPaths.back());

		const auto& cubeMapIt = RM->cubeMaps.find(pathsDir);

		// Check if the Texture is already loaded
		if (cubeMapIt != RM->cubeMaps.end())
		{
			return cubeMapIt->second;
		}

		return RM->cubeMaps[pathsDir] = std::make_shared<CubeMap>(cubeMapPaths);
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

		return RM->materials[materialPath] = std::make_shared<Material>();
	}

	std::shared_ptr<Recipe> ResourcesManager::loadRecipe(const std::string& recipePath)
	{
		ResourcesManager* RM = instance();

		const auto& recipeIt = RM->recipes.find(recipePath);

		// Check if the Material is already loaded
		if (recipeIt != RM->recipes.end())
			return recipeIt->second;

		return RM->recipes[recipePath] = std::make_shared<Recipe>(recipePath);
	}

	void addData(std::vector<Core::Maths::vec3>& dataVector, std::istringstream& iss)
	{
		// Get a 3D Vector data form string stream
		Core::Maths::vec3 data = { 0.f };

		iss >> data.x;
		iss >> data.y;
		iss >> data.z;

		dataVector.push_back(data);
	}

	LowRenderer::Color getColor(std::istringstream& iss)
	{
		// Get a Color data form string stream
		LowRenderer::Color color = { 0.f };

		iss >> color.data.r;
		iss >> color.data.g;
		iss >> color.data.b;

		return color;
	}

	// Use to know if needed to triangulate faces
	int getNumFace(const std::string& line)
	{
		int numFace = 0;

		// Check how many spaces the line has
		for (size_t i = 0; i < line.length(); i++)
		{
			if (line[i] == ' ')
				numFace++;
		}

		return numFace;
	}

	// Give the number of vertices of a face from an .obj
	int numVerticesFace(const std::string& line)
	{
		int numV = 0;

		for (size_t i = 0; i < line.length(); i++)
		{
			if (line[i] == '/')
				numV++;
		}

		if (numV > 6)
			return 4;
		else
			return 3;
	}

	// Allow to know if face is of type v//vn or v/uv/vn
	int getFaceType(const std::string& line)
	{
		for (size_t i = 3; i < line.length(); i++)
		{
			if (line[i - 1] == '/' && line[i] == '/')
				return 1;
		}

		return 0;
	}

	void addIndices(std::vector<unsigned int>& indices, std::istringstream& iss, const std::string& line)
	{
		unsigned int indicesVertices[4];
		unsigned int indicesUV[4];
		unsigned int indicesNormals[4];

		// Number of vertices of the face 
		int numV = numVerticesFace(line);

		// Data type of the face
		int type = getFaceType(line);

		for (int i = 0; i < numV; i++)
		{
			// Face of type v/uv/vn
			if (type == 0)
			{
				iss >> indicesVertices[i];
				iss.ignore();
				iss >> indicesUV[i];
				iss.ignore();
				iss >> indicesNormals[i];
				iss.ignore();
			}
			// Face of type v//vn
			else
			{
				iss >> indicesVertices[i];
				iss.ignore();
				iss.ignore();
				iss >> indicesNormals[i];

				indicesUV[i] = 1;
			}

			int indexV2Strip = i;
			// Strip faces (triangulation)
			if (i > 2)
			{
				indices.push_back(indicesVertices[0] - 1);
				indices.push_back(indicesUV[0] - 1);
				indices.push_back(indicesNormals[0] - 1);
				indices.push_back(indicesVertices[i - 1] - 1);
				indices.push_back(indicesUV[i - 1] - 1);
				indices.push_back(indicesNormals[i - 1] - 1);
			}

			// Add vertex indices to the mesh indices
			indices.push_back(indicesVertices[i] - 1);
			indices.push_back(indicesUV[i] - 1);
			indices.push_back(indicesNormals[i] - 1);
		}
	}

	void ResourcesManager::loadMaterialsFromMtl(const std::string& dirPath, const std::string& mtlName)
	{
		ResourcesManager* RM = instance();

		std::string filePath = dirPath + mtlName;

		// Check if the file exist
		std::ifstream dataMat(filePath.c_str());
		if (!dataMat)
		{
			Core::Debug::Log::error("Unable to read the file: " + filePath);
			dataMat.close();
			return;
		}

		Material mat;
		std::string line;
		bool isFirstMat = true;

		Core::Debug::Log::info("Loading materials at " + filePath);

		// Get all mesh materials
		while (std::getline(dataMat, line))
		{
			std::istringstream iss(line);
			std::string type;
			iss >> type;

			if (type == "#" || type == "" || type == "\n")
				continue;

			if (type == "newmtl")
			{
				if (isFirstMat)
					isFirstMat = false;
				else
				{
					// Add the material
					*ResourcesManager::loadMaterial(mat.m_name) = mat;
					mat = Material();
				}

				iss >> mat.m_name;

				continue;
			}
			else if (type == "Ns")
				iss >> mat.shininess;
			else if (type == "Ka")
				mat.ambient = getColor(iss);
			else if (type == "Kd")
				mat.diffuse = getColor(iss);
			else if (type == "Ks")
				mat.specular = getColor(iss);
			else if (type == "Ke")
				mat.emissive = getColor(iss);
			else if (type == "Ni")
				iss >> mat.opticalDensity;
			else if (type == "d")
				iss >> mat.transparency;
			else if (type == "illum")
			{
				iss >> mat.illumination;
				continue;
			}
			
			std::string texName;
			iss >> texName;

			// Load mesh textures
			if (type == "map_Ka")
				mat.ambientTex  = ResourcesManager::loadTexture(dirPath + Utils::getFileNameFromPath(texName));
			else if (type == "map_Kd")
				mat.diffuseTex  = ResourcesManager::loadTexture(dirPath + Utils::getFileNameFromPath(texName));
			else if (type == "map_Ks")
				mat.specularTex = ResourcesManager::loadTexture(dirPath + Utils::getFileNameFromPath(texName));
			else if (type == "map_Ke")
				mat.emissiveTex = ResourcesManager::loadTexture(dirPath + Utils::getFileNameFromPath(texName));
			else if (type == "map_d")
				mat.alphaTex    = ResourcesManager::loadTexture(dirPath + Utils::getFileNameFromPath(texName));
		}

		// Add the material
		*ResourcesManager::loadMaterial(mat.m_name) = mat;

		dataMat.close();
	}

	// Load an obj with mtl (do triangulation)
	void ResourcesManager::loadObj(const std::string& filePath)
	{
		ResourcesManager* RM = instance();

		// Check if the object is already loaded
		if (RM->childrenMeshes.find(filePath) != RM->childrenMeshes.end())
		{
			Core::Debug::Log::info("Model at " + filePath + " is already loaded");
			return;
		}

		std::ifstream dataObj(filePath.c_str());

		// Check if the file exists
		if (!dataObj)
		{
			Core::Debug::Log::error("Unable to read the file : " + filePath);
			dataObj.close();
			return;
		}

		Core::Debug::Log::info("Start loading obj " + filePath);

		std::vector<Core::Maths::vec3> vertices;
		std::vector<Core::Maths::vec3> texCoords;
		std::vector<Core::Maths::vec3> normals;
		std::vector<unsigned int> indices;
		std::vector<std::string> names;
		std::string dirPath = Utils::getDirectory(filePath);

		bool isFirstObject = true;
		Resources::Mesh mesh;

		Core::Debug::Log::info("Loading meshes");

		std::string line;
		while (std::getline(dataObj, line))
		{
			std::istringstream iss(line);
			std::string type;

			iss >> type;

			if (type == "#" || type == "" || type == "\n")
				continue;

			if (type == "o")
			{
				if (isFirstObject)
					isFirstObject = false;
				else
				{
					// Compute and add the mesh
					mesh.compute(vertices, texCoords, normals, indices);
					RM->meshes[mesh.name] = std::make_shared<Mesh>(mesh);
					names.push_back(mesh.name);

					mesh = Mesh();
					indices.clear();
				}

				iss >> mesh.name;
			}
			else if (type == "v")
				addData(vertices, iss);
			else if (type == "vt")
				addData(texCoords, iss);
			else if (type == "vn")
				addData(normals, iss);
			else if (type == "f")
				addIndices(indices, iss, line);
			else if (type == "usemtl")
			{
				std::string matName;
				iss >> matName;

				RM->childrenMaterials[mesh.name] = matName;
			}
			else if (type == "mtllib")
			{
				std::string mtlName;
				iss >> mtlName;

				// Load mtl file
				ResourcesManager::loadMaterialsFromMtl(dirPath, mtlName);
			}
		}

		// Compute and add the mesh
		mesh.compute(vertices, texCoords, normals, indices);
		RM->meshes[mesh.name] = std::make_shared<Mesh>(mesh);
		names.push_back(mesh.name);

		RM->childrenMeshes[filePath] = names;

		dataObj.close();

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
}