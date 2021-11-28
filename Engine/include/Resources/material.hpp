#pragma once
#include <memory>

#include "color.hpp"

#include "resource.hpp"
#include "texture.hpp"
#include "shader.hpp"

namespace Resources
{
	struct Material : public Resource
	{
		Material(const std::string& name);

		LowRenderer::Color ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
		LowRenderer::Color diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
		LowRenderer::Color specular = { 0.0f, 0.0f, 0.0f, 1.0f };
		LowRenderer::Color emissive = { 0.0f, 0.0f, 0.0f, 0.0f };

		std::shared_ptr<Texture> alphaTex		= Texture::defaultAlpha;
		std::shared_ptr<Texture> ambientTex		= Texture::defaultAmbient;
		std::shared_ptr<Texture> diffuseTex		= Texture::defaultDiffuse;
		std::shared_ptr<Texture> emissiveTex	= Texture::defaultEmissive;
		std::shared_ptr<Texture> specularTex	= Texture::defaultSpecular;
		std::shared_ptr<Texture> normalMap		= Texture::defaultNormalMap;

		float shininess = 100.f;
		float opticalDensity = 1.f;
		float transparency = 0.f;
		float illumination = 0.f;

		void parse(const std::string& toParse, const std::string& directoryPath);

		static std::shared_ptr<Material> defaultMaterial;

		void sendToShader(const std::shared_ptr<Resources::ShaderProgram>& shaderProgram) const;
		void bindTextures() const;

		void drawImGui();
	};
}