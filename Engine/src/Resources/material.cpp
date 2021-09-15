#include "material.hpp"

#include "resources_manager.hpp"
#include "utils.hpp"
#include "maths.hpp"

namespace Resources
{
	std::shared_ptr<Material> Material::defaultMaterial = nullptr;

	void Material::sendToShader(const std::shared_ptr<ShaderProgram>& shaderProgram) const
	{
		// Set the model's material informations 
		shaderProgram->setUniform("material.ambient", &ambient);
		shaderProgram->setUniform("material.diffuse", &diffuse);
		shaderProgram->setUniform("material.specular", &specular);
		shaderProgram->setUniform("material.emissive", &emissive);
		
		shaderProgram->setUniform("material.shininess", &shininess);

		// Set the textures' location of the shader program
		std::vector<std::string> shaderName =
		{ "material.alphaTexture", "material.ambientTexture", "material.diffuseTexture",
			"material.emissiveTexture", "material.specularTexture"};

		for (int i = 0; i < 5; i++)
			shaderProgram->setUniform(shaderName[i], &i);
	}

	void Material::bindTextures() const
	{
		if (alphaTex)
			if (!alphaTex->bind(0))
				Texture::defaultAlpha->bind(0);

		if (ambientTex)
			if (!ambientTex->bind(1))
				Texture::defaultAmbient->bind(1);

		if (diffuseTex)
			if (!diffuseTex->bind(2))
				Texture::defaultDiffuse->bind(2);
			
		if (emissiveTex)
			if (!emissiveTex->bind(3))
				Texture::defaultEmissive->bind(3);

		if (specularTex)
			if (!specularTex->bind(4))
				Texture::defaultSpecular->bind(4);

		glActiveTexture(0);
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

	void loadMaterialsFromMtl(const std::string& dirPath, const std::string& mtlName)
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
					*ResourcesManager::getMaterial(mat.m_name) = mat;
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
				mat.ambientTex = ResourcesManager::getTexture(dirPath + Utils::getFileNameFromPath(texName));
			else if (type == "map_Kd")
				mat.diffuseTex = ResourcesManager::getTexture(dirPath + Utils::getFileNameFromPath(texName));
			else if (type == "map_Ks")
				mat.specularTex = ResourcesManager::getTexture(dirPath + Utils::getFileNameFromPath(texName));
			else if (type == "map_Ke")
				mat.emissiveTex = ResourcesManager::getTexture(dirPath + Utils::getFileNameFromPath(texName));
			else if (type == "map_d")
				mat.alphaTex = ResourcesManager::getTexture(dirPath + Utils::getFileNameFromPath(texName));
		}

		// Add the material
		*ResourcesManager::getMaterial(mat.m_name) = mat;

		dataMat.close();
	}
}