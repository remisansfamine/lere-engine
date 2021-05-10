#pragma once

#include "renderer.hpp"
#include "texture.hpp"
#include "mesh.hpp"

namespace LowRenderer
{
	class SpriteRenderer : public Renderer
	{
	private:
		std::shared_ptr<Resources::Mesh> mesh = nullptr;
		std::shared_ptr<Resources::Texture> texture = nullptr;
	public:
		SpriteRenderer(Engine::GameObject& gameObject, const std::string& shaderPromgramName);
		SpriteRenderer(Engine::GameObject& gameObject, const std::string& shaderPromgramName, const std::string& texturePath);
		SpriteRenderer(Engine::GameObject& gameObject, const std::shared_ptr<SpriteRenderer>& ptr, const std::string& shaderPromgramName);

		void draw() const override;
		void drawImGui() override;
		std::string toString() override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}