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
	private:
		GLuint textureID = 0;
		
		int width = 0;
		int height = 0;
		float* colorBuffer = nullptr;
		bool stbiLoaded = false;
		bool loadDone = false;


	public:
		Texture() = default;
		Texture(const std::string& filePath);
		Texture(const std::string& name, int width, int height, float* colorBuffer);
		~Texture();

		bool generateBuffer(const std::string& filePath);
		bool generateID();

		void mainThreadInitialization() override;

		GLuint getID() const;

		bool bind(int textureIndex) const;

		static std::shared_ptr<Texture> defaultAlpha;
		static std::shared_ptr<Texture> defaultAmbient;
		static std::shared_ptr<Texture> defaultDiffuse;
		static std::shared_ptr<Texture> defaultEmissive;
		static std::shared_ptr<Texture> defaultSpecular;
	};	
}
