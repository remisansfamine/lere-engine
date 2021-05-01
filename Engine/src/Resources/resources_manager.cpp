#include <fstream>
#include <sstream>

#include "resources_manager.hpp"

#include "debug.hpp"

#include "maths.hpp"

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

	void ResourcesManager::init()
	{
		ResourcesManager* RM = instance();

		if (RM->initialized)
		{
			Core::Debug::Log::error("The Resources Manager is already initialized");
			return;
		}

		// TODO : Add basic materials/models/textures

		RM->initialized = true;

		Core::Debug::Log::info("Resources Manager initialized");

		loadShaderProgram("testShader", "resources/shaders/testShader.vert", "resources/shaders/testShader.frag");
	}

	std::shared_ptr<Scene> ResourcesManager::loadScene(const std::string& scenePath)
	{
		ResourcesManager* RM = instance();
		
		const auto& sceneIt = RM->scenes.find(scenePath);

		if (sceneIt != RM->scenes.end())
		{
			return sceneIt->second;
		}

		// TODO: Add Warn load fail
		return RM->scenes[scenePath] = std::make_shared<Scene>(Scene(scenePath));
	}

	std::shared_ptr<Shader> ResourcesManager::loadShader(const std::string& shaderPath)
	{
		ResourcesManager* RM = instance();
		
		const auto& shaderIt = RM->shaders.find(shaderPath);

		if (shaderIt != RM->shaders.end())
		{
			return shaderIt->second;
		}

		// TODO: Add Warn load fail
		return RM->shaders[shaderPath] = std::make_shared<Shader>(Shader(shaderPath));
	}

	std::shared_ptr<ShaderProgram> ResourcesManager::loadShaderProgram(const std::string& programName, const std::string& vertPath, const std::string& fragPath)
	{
		ResourcesManager* RM = instance();

		const auto& programIt = RM->shaderPrograms.find(programName);

		if (programIt != RM->shaderPrograms.end())
		{
			return programIt->second;
		}

		// TODO: Add Warn load fail
		return RM->shaderPrograms[programName] = std::make_shared<ShaderProgram>(ShaderProgram(programName, vertPath, fragPath));
	}

	std::shared_ptr<Texture> ResourcesManager::loadTexture(const std::string& texturePath)
	{
		ResourcesManager* RM = instance();

		const auto& textureIt = RM->textures.find(texturePath);

		if (textureIt != RM->textures.end())
		{
			return textureIt->second;
		}

		// TODO: Add Warn load fail
		return RM->textures[texturePath] = std::make_shared<Texture>(Texture(texturePath));
	}

	std::shared_ptr<Material> ResourcesManager::loadMaterial(const std::string& materialPath)
	{
		ResourcesManager* RM = instance();

		const auto& materialIt = RM->materials.find(materialPath);

		if (materialIt != RM->materials.end())
		{
			return materialIt->second;
		}

		// TODO: Add Warn load fail
		return RM->materials[materialPath] = std::make_shared<Material>(Material());
	}

	void addVertices(std::vector<Core::Maths::vec3>& vertices, std::istringstream& iss)
	{
		Core::Maths::vec3 vertice;

		iss >> vertice.x;
		iss >> vertice.y;
		iss >> vertice.z;

		vertices.push_back(vertice);

		return;
	}

	// Add a texture coordinate from an .obj
	void addTexCoords(std::vector<Core::Maths::vec3>& texCoordsVec, std::istringstream& iss)
	{
		Core::Maths::vec3 texCoords;

		iss >> texCoords.x;
		iss >> texCoords.y;
		iss >> texCoords.z;

		texCoordsVec.push_back(texCoords);

		return;
	}

	// Add a vertice normal from an .obj
	void addNormals(std::vector<Core::Maths::vec3>& normalsVec, std::istringstream& iss)
	{
		Core::Maths::vec3 normals;

		iss >> normals.x;
		iss >> normals.y;
		iss >> normals.z;

		normalsVec.push_back(normals);

		return;
	}

	// Use to know if needed to triangulate faces
	int getNumFace(const std::string& line)
	{
		int numFace = 0;
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

	// Load an obj with mtl (do triangulation)
	void ResourcesManager::loadObj(const std::string& filePath)
	{
		ResourcesManager* RM = instance();
		/*if (isObjLoad(nameMeshes, filePath))
		{
			Core::Debug::Log::info("Model at " + filePath + " is already load");
			return;
		}*/

		std::ifstream dataObj(filePath.c_str());

		if (!dataObj)
		{
			Core::Debug::Log::error("ERROR: Unable to read the file : " + filePath + ")");
			dataObj.close();
			return;
		}

		Core::Debug::Log::info("Start loading obj " + filePath);

		std::vector<Core::Maths::vec3> vertices;
		std::vector<Core::Maths::vec3> texCoords;
		std::vector<Core::Maths::vec3> normals;
		std::vector<unsigned int> indices;
		std::vector<std::string> names;

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
					mesh.compute(vertices, texCoords, normals, indices);
					RM->meshes[mesh.name] = std::make_shared<Mesh>(mesh);
					names.push_back(mesh.name);

					mesh = Mesh();
					indices.clear();
				}

				iss >> mesh.name;
				continue;
			}
			else if (type == "v")
				addVertices(vertices, iss);
			else if (type == "vt")
				addTexCoords(texCoords, iss);
			else if (type == "vn")
				addNormals(normals, iss);
			else if (type == "f")
				addIndices(indices, iss, line);

			iss.clear();
		}

		mesh.compute(vertices, texCoords, normals, indices);
		RM->meshes[mesh.name] = std::make_shared<Mesh>(mesh);
		names.push_back(mesh.name);

		RM->modelChildren[filePath] = names;

		dataObj.close();

		Core::Debug::Log::info("Finish loading obj " + filePath);

		return;
	}

	std::vector<std::string>* ResourcesManager::getMeshNames(const std::string& filePath)
	{
		ResourcesManager* RM = instance();

		auto meshNameIt = RM->modelChildren.find(filePath);

		if (meshNameIt == RM->modelChildren.end())
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

		if (meshIt == RM->meshes.end())
		{
			Core::Debug::Log::error("Can not find mesh named " + meshName);
			return nullptr;
		}

		return meshIt->second;
	}
}