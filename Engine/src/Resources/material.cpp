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

		// Set the textures' location of the shader program
		std::vector<std::string> textureName =
		{ "material.alphaTexture", "material.ambientTexture", "material.diffuseTexture",
			"material.emissiveTexture", "material.specularTexture", "material.normalMap" };

		for (int i = 0; i < textureName.size(); i++)
			shaderProgram->setUniform(textureName[i], &i, false);
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

		if (normalMap)
			if (!normalMap->bind(5))
				Texture::defaultNormalMap->bind(5);

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

			if (ImGui::TreeNode("Alpha texture:"))
			{
				alphaTex->drawImGui();
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Ambient texture:"))
			{
				ambientTex->drawImGui();
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Diffuse texture:"))
			{
				diffuseTex->drawImGui();
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Emissive texture:"))
			{
				emissiveTex->drawImGui();
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Specular texture:"))
			{
				specularTex->drawImGui();
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Normal texture:"))
			{
				normalMap->drawImGui();
				ImGui::TreePop();
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

			std::shared_ptr<Texture>* texturePtr;

			// Load mesh textures
			if (type == "map_Ka")
				texturePtr = &ambientTex;
			else if (type == "map_Kd")
				texturePtr = &diffuseTex;
			else if (type == "map_Ks")
				texturePtr = &specularTex;
			else if (type == "map_Ke")
				texturePtr = &emissiveTex;
			else if (type == "map_d")
				texturePtr = &alphaTex;
			else if (type == "map_bump")
				texturePtr = &normalMap;
			else
				continue;

			*texturePtr = ResourcesManager::loadTexture(directoryPath + Utils::getFileNameFromPath(texName));
		}
	}
}