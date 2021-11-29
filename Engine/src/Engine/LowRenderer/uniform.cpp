#include "uniform.hpp"

#include <stdio.h>
#include <string>

namespace LowRenderer
{
	bool Uniform::bind(const void* value, int count, bool transpose) const
	{
        if (location < 0)
            return false;

        switch (type)
        {
        case GL_INT:
        case GL_BOOL:
        case GL_SAMPLER_2D:
        case GL_SAMPLER_CUBE:
            glUniform1iv(location, count, (GLint*)value);
            return true;

        case GL_INT_VEC2:
        case GL_BOOL_VEC2:
            glUniform2iv(location, count, (GLint*)value);
            return true;

        case GL_INT_VEC3:
        case GL_BOOL_VEC3:
            glUniform3iv(location, count, (GLint*)value);
            return true;

        case GL_INT_VEC4:
        case GL_BOOL_VEC4:
            glUniform4iv(location, count, (GLint*)value);
            return true;

        case GL_FLOAT:
            glUniform1fv(location, count, (GLfloat*)value);
            return true;

        case GL_FLOAT_VEC2:
            glUniform2fv(location, count, (GLfloat*)value);
            return true;

        case GL_FLOAT_VEC3:
            glUniform3fv(location, count, (GLfloat*)value);
            return true;

        case GL_FLOAT_VEC4:
            glUniform4fv(location, count, (GLfloat*)value);
            return true;

        case GL_FLOAT_MAT2:
            glUniformMatrix2fv(location, count, transpose, (GLfloat*)value);
            return true;

        case GL_FLOAT_MAT3:
            glUniformMatrix3fv(location, count, transpose, (GLfloat*)value);
            return true;

        case GL_FLOAT_MAT4:
            glUniformMatrix4fv(location, count, transpose, (GLfloat*)value);
            return true;

        default:
            // return false if the type is not supported
            return false;
        }
	}

    Uniform::Uniform(GLint location, GLenum type)
        : location(location), type(type)
    {

    }

    bool Uniform::areSame(const void* value)
    {
        return currentValue == value || memcmp(currentValue, value, typeSize);
    }

    UniformBlock::UniformBlock(GLuint bindingPoint, GLsizei UBOsize)
        : bindingPoint(bindingPoint)
    {
        glGenBuffers(1, &UBO);

        // TODO: REMOVE THIS SHIT
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferData(GL_UNIFORM_BUFFER, UBOsize, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, UBO, 0, UBOsize);
    }

    UniformBlock::~UniformBlock()
    {
        glDeleteBuffers(1, &UBO);
    }

    void UniformBlock::bind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    }

    void UniformBlock::unbind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}