#include "material.hpp"

namespace Resources
{
	Material Material::defaultMaterial = Material();

	void Material::sendToShader(const std::shared_ptr<ShaderProgram>& shaderProgram)
	{
		// Set the model's material informations 
		shaderProgram->setUniform("material.ambient", &ambient);
		shaderProgram->setUniform("material.diffuse", &diffuse);
		shaderProgram->setUniform("material.specular", &specular);
		shaderProgram->setUniform("material.emissive", &emissive);

		shaderProgram->setUniform("material.shininess", &shininess);
	}

	void Material::bindTextures()
	{
		// TODO : set textures with uniform
		ambientTex->bind(0);
		diffuseTex->bind(1);
		specularTex->bind(2);
		emissiveTex->bind(3);
		dissolveTex->bind(4);
	}
}