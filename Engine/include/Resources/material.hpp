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
		LowRenderer::Color ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
		LowRenderer::Color diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
		LowRenderer::Color specular = { 0.0f, 0.0f, 0.0f, 1.0f };
		LowRenderer::Color emissive = { 0.0f, 0.0f, 0.0f, 0.0f };

		std::shared_ptr<Resources::Texture> ambientTex  = nullptr;
		std::shared_ptr<Resources::Texture> diffuseTex  = nullptr;
		std::shared_ptr<Resources::Texture> specularTex = nullptr;
		std::shared_ptr<Resources::Texture> emissiveTex = nullptr;
		std::shared_ptr<Resources::Texture> dissolveTex = nullptr;

		float shininess = 100.f;

		static Material defaultMaterial;

		void sendToShader(const std::shared_ptr<Resources::ShaderProgram>& shaderProgram);
		void bindTextures();
	};
}