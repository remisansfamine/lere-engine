#pragma once

#include <string>
#include <memory>

#include <glad/glad.h>

#include "resource.hpp"

#include "maths.hpp"

namespace Resources
{
	class Texture : public Resource
	{
	protected:
		GLuint textureID = 0;
		
		int		width = 0;
		int		height = 0;
		float*	colorBuffer = nullptr;
		bool	stbiLoaded = false;

		void mainThreadInitialization() override;

	public:
		Texture() = default;
		Texture(const std::string& filePath);
		Texture(const std::string& name, int width, int height, float* colorBuffer);
		~Texture();

		virtual bool generateBuffer();
		virtual bool generateID();

		GLuint getID() const;
		int getHeight() const;
		int getWidth() const;

		void drawImGui();

		bool bind(int textureIndex) const;

		static std::shared_ptr<Texture> defaultAlpha;
		static std::shared_ptr<Texture> defaultAmbient;
		static std::shared_ptr<Texture> defaultDiffuse;
		static std::shared_ptr<Texture> defaultEmissive;
		static std::shared_ptr<Texture> defaultSpecular;
	};

	class CubeMapTexture : public Texture
	{
	public:
		CubeMapTexture(int ID, const std::string& filePath);
		CubeMapTexture() = default;

		int cubeMapID;

		bool generateBuffer() override;
		bool generateID() override;
	};
}
