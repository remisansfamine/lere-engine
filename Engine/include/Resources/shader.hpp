#pragma once

#include <unordered_map>
#include <memory>

#include <glad/glad.h>

#include "resource.hpp"

#include "uniform.hpp"

namespace Resources
{
	class Shader : public Resource
	{
	private:
		std::string shaderCode;
		GLenum type;

		bool setID();
		void setCode();
		void compile();
		void mainThreadInitialization() override;

		void destroy();
		void create();
	public:
		GLint shaderID = GL_INVALID_VALUE;

		void load();
		void reload();

		Shader(const std::string& shaderPath);
		~Shader();

		void drawImGui();
	};

	class ShaderProgram : public Resource
	{
	private:
		bool debugSetUniform = false;

		std::shared_ptr<Shader> vertShader;
		std::shared_ptr<Shader> fragShader;
		std::shared_ptr<Shader> geomShader;

		GLint programID = GL_INVALID_VALUE;
		std::string name;

		std::unordered_map<std::string, LowRenderer::Uniform> uniforms;

		void loadUniforms();
		void loadLocations();

		void mainThreadInitialization() override;

		void create();
		void destroy();

	public:
		ShaderProgram(const std::string& programName, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
		~ShaderProgram();

		void setUniform(const std::string& target, const void* value, bool shouldBeTracked, int count = 1, bool transpose = false) const;
		bool bind() const;
		void unbind() const;

		void reload();

		std::string getName();

		void linkShaders();

		bool bindToUBO(const std::string& UBOName, GLint UBOBindID);

		void drawImGui();
	};
}