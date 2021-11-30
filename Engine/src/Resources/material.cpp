#include "material.hpp"

#include <imgui.h>

#include "resources_manager.hpp"
#include "utils.hpp"
#include "maths.hpp"

namespace Resources
{
	std::shared_ptr<Material> Material::defaultMaterial = nullptr;

	Material::Material(const std::string& name)
		: Resource(name)
	{

	}

	void Material::sendToShader(const std::shared_ptr<ShaderProgram>& shaderProgram) const
	{
		// Set the model's material informations 
		shaderProgram->setUniform("material.ambient", &ambient, false);
		shaderProgram->setUniform("material.diffuse", &diffuse, false);
		shaderProgram->setUniform("material.specular", &specular, false);
		shaderProgram->setUniform("material.emissive", &emissive, false);
		
		shaderProgram->setUniform("material.shininess", &shininess, false);
		shaderProgram->setUniform("material.refractiveIndex", &opticalDensity, false);

		for (auto [textureName, texturePtr] : textures)
		{
			if (!texturePtr)
				continue;

			if (!shaderProgram->setSampler(textureName, texturePtr->getID()))
				shaderProgram->setSampler(textureName, Material::defaultMaterial->textures[textureName]->getID());
		}

		glActiveTexture(0);
	}

	void Material::drawImGui()
	{
		if (ImGui::TreeNode(m_filePath.c_str()))
		{
			ImGui::ColorEdit4("Ambient color", &ambient.data.x);
			ImGui::ColorEdit4("Diffule color", &diffuse.data.x);
			ImGui::ColorEdit4("Specular color", &specular.data.x);
			ImGui::ColorEdit4("Emissive color", &emissive.data.x);

			ImGui::DragFloat("Shininess", &shininess);
			ImGui::DragFloat("OpticalDensity", &opticalDensity);
			ImGui::DragFloat("Transparency", &transparency);
			ImGui::DragFloat("Illumination", &illumination);

			for (auto [textureName, texturePtr] : textures)
			{
				if (!texturePtr)
					continue;

				if (ImGui::TreeNode(textureName.c_str()))
				{
					texturePtr->drawImGui();
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
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

	void Material::parse(const std::string& toParse, const std::string& directoryPath)
	{
		std::istringstream stringStream(toParse);

		std::string line;
		while (std::getline(stringStream, line))
		{
			std::istringstream iss(line);
			std::string type;
			iss >> type;

			if (type == "#" || type == "" || type == "\n" || type == "n")
				continue;

			if (type == "Ns")
			{
				iss >> shininess;
				continue;
			}
			if (type == "Ka")
			{
				ambient = getColor(iss);
				continue;
			}
			if (type == "Kd")
			{
				diffuse = getColor(iss);
				continue;
			}
			if (type == "Ks")
			{
				specular = getColor(iss);
				continue;
			}
			if (type == "Ke")
			{
				emissive = getColor(iss);
				continue;
			}
			if (type == "Ni")
			{
				iss >> opticalDensity;
				continue;
			}
			if (type == "d")
			{
				iss >> transparency;
				continue;
			}
			if (type == "illum")
			{
				iss >> illumination;
				continue;
			}

			std::string texName;
			iss >> texName;

			std::shared_ptr<Texture> texturePtr = ResourcesManager::loadTexture(directoryPath + Utils::getFileNameFromPath(texName));

			// Load mesh textures
			if (type == "map_d")
				textures["material.alphaTexture"] = texturePtr;
			else if (type == "map_Ka")
				textures["material.ambientTexture"] = texturePtr;
			else if (type == "map_Kd")
				textures["material.diffuseTexture"] = texturePtr;
			else if (type == "map_Ke")
				textures["material.emissiveTexture"] = texturePtr;
			else if (type == "map_Ks")
				textures["material.specularTexture"] = texturePtr;
			else if (type == "map_bump")
				textures["material.normalMap"] = texturePtr;
		}
	}
}