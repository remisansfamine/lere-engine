#pragma once

#include <glad/glad.h>

namespace LowRenderer
{
	class Uniform
	{
		GLint  location;
		GLenum type;

		void* currentValue = nullptr;
		size_t typeSize = 0u;

	public:
		Uniform() = default;
		Uniform(GLint location, GLenum type);

		bool bind(const void* value, int count, bool transpose) const;

		bool areSame(const void* value);
	};

	class UniformBlock
	{
		GLuint	bindingPoint;
		GLuint	UBO;

	public:
		UniformBlock() = default;
		UniformBlock(GLuint bindingPoint, GLsizei UBOsize);
		~UniformBlock();

		void bind();
		void unbind();
	};
}