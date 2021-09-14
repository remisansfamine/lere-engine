#pragma once

#include <unordered_map>
#include <memory>

#include <glad/glad.h>

#include "resource.hpp"

namespace Resources
{
	struct Uniform
	{
		GLint  location;
		GLenum type;
	};

	class Shader : public Resource
	{
	private:
		void setID();
		void setCode();
		void compile();

		void mainThreadInitialization() override;

		std::string shaderCode;

	public:
		GLint shaderID = GL_INVALID_VALUE;

		Shader(const std::string& shaderPath);
		~Shader();
	};

	class ShaderProgram : public Resource
	{
	private:
		std::shared_ptr<Shader> vertShader;
		std::shared_ptr<Shader> fragShader;
		std::shared_ptr<Shader> geomShader;

		GLint programID = GL_INVALID_VALUE;
		std::string name;

		std::unordered_map<std::string, Uniform> uniforms;

		void loadLocations();

		void mainThreadInitialization() override;

	public:
		ShaderProgram(const std::string& programName, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
		~ShaderProgram();

		void setUniform(const std::string& target, const void* value, int count = 1, bool transpose = false) const;
		void bind() const;
		void unbind() const;

		std::string getName();

		void linkShaders();
	};
}