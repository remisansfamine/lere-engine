#include "shader.hpp"

#include <fstream>
#include <filesystem>

#include "resources_manager.hpp"
#include "thread_pool.hpp"
#include "debug.hpp"

#include "utils.hpp"


namespace Resources
{
#pragma region SHADER
	Shader::Shader(const std::string& shaderPath)
        : Resource(shaderPath)
	{
		Core::Debug::Log::info("Loading " + shaderPath);

        setID();

        //ThreadPool::addTask(std::bind(&Shader::setCode, this));
        setCode();
	}

    Shader::~Shader()
    {
        glDeleteShader(shaderID);
    }

    void Shader::setID()
    {
        // Check if the file can be read
        if (std::filesystem::exists(m_filePath))
        {
            // Create the shader by checking its extension
            if (Utils::hasSuffix(m_filePath, ".vert"))
                shaderID = glCreateShader(GL_VERTEX_SHADER);

            else if (Utils::hasSuffix(m_filePath, ".frag"))
                shaderID = glCreateShader(GL_FRAGMENT_SHADER);

            else if (Utils::hasSuffix(m_filePath, ".geom"))
                shaderID = glCreateShader(GL_GEOMETRY_SHADER);

            else
                Core::Debug::Log::error("File extension is not compatible");

            return;
        }

        Core::Debug::Log::error("Cannot open the file " + m_filePath);
    }

    void Shader::setCode()
    {
        std::ifstream ifs(m_filePath);

        // Send the code to OpenGL as a char*
        shaderCode.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

        ResourcesManager::addToMainThreadInitializerQueue(this);
    }

    void Shader::compile()
    {
        const char* code = shaderCode.c_str();

        glShaderSource(shaderID, 1, &code, NULL);
        glCompileShader(shaderID);

        shaderCode.clear();

        // Check if the compilation is a success
        GLint success;
        const GLsizei infoSize = 256;
        GLchar infoLog[infoSize];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shaderID, infoSize, NULL, infoLog);
            Core::Debug::Log::error(m_filePath + " shader compilation failed" + infoLog);
        }
    }

    void Shader::mainThreadInitialization()
    {
        compile();
    }
#pragma endregion


#pragma region SHADER_PROGRAM
	ShaderProgram::ShaderProgram(const std::string& programName, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath)
        : programID(glCreateProgram()), name(programName)
	{
        Core::Debug::Log::info("Loading program " + programName);

        Core::Debug::Log::info("Linking " + vertPath + " and " + fragPath + " to " + programName);

        vertShader = Resources::ResourcesManager::getShader(vertPath);
        fragShader = Resources::ResourcesManager::getShader(fragPath);

        if (geomPath != "")
            geomShader = Resources::ResourcesManager::getShader(geomPath);

        ResourcesManager::addToMainThreadInitializerQueue(this);
	}

    void ShaderProgram::linkShaders()
    {
        // Attach the two shaders and link them
        glAttachShader(programID, vertShader->shaderID);
        glAttachShader(programID, fragShader->shaderID);

        if (geomShader)
            glAttachShader(programID, geomShader->shaderID);

        glLinkProgram(programID);

        // Check if there is an error
        GLint success;
        const GLsizei infoSize = 256;
        GLchar infoLog[infoSize];
        glGetProgramiv(programID, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(programID, infoSize, NULL, infoLog);
            Core::Debug::Log::error(m_name + " shader program linking failed: " + infoLog);
            return;
        }

        // If there is no error, load the uniform locations of the program
        loadLocations();
    }

    void ShaderProgram::mainThreadInitialization()
    {
        linkShaders();
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(programID);
    }

    void ShaderProgram::loadLocations()
    {
        uniforms.clear();

        // Get the active uniforms count
        GLint uniformCount;
        glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &uniformCount);

        // Loop over the active uniforms
        for (GLint i = 0; i < uniformCount; i++)
        {
            GLsizei nameLength;
            GLint size;             // Size of the variable
            GLenum type;            // Type of the variable
            const GLsizei bufSize = 256;
            GLchar uniName[bufSize];

            // Get the current uniform informations
            glGetActiveUniform(programID, i, bufSize, &nameLength, &size, &type, uniName);

            GLint location = glGetUniformLocation(programID, uniName);

            if (location < 0)
            {
                Core::Debug::Log::error("Cannot find the uniform named: " + std::string(uniName) + " - Location not valid");
                continue;
            }

            // Create a new uniform with the location and the type
            // And add it to a map
            uniforms[uniName] = { location, type };
        }

        int test = 0;
    }

    void ShaderProgram::setUniform(const std::string& target, const void* value,
                                   int count, bool transpose) const
    {
        // Get the iterator with the target
        const auto& currentIt = uniforms.find(target);

        // Check if this iterator is valid
        if (currentIt == uniforms.end())
        {
            Core::Debug::Log::error("Cannot find the uniform named: " + target + " - There is no uniform named like that");
            return;
        }

        const Uniform& uniform = currentIt->second;

        // Check if the location is valid
        if (uniform.location < 0)
        {
            Core::Debug::Log::error("Cannot find the uniform named: " + target + " - Location not valid");
            return;
        }

        // Call the correct function in function of the uniform's type
        #pragma region Uniform seter
        switch (uniform.type)
        {
            case GL_INT:
            case GL_BOOL:
            case GL_SAMPLER_2D_ARB:
            case GL_SAMPLER_CUBE_ARB:
                glUniform1iv(uniform.location, count, (GLint*)value);
                break;

            case GL_INT_VEC2:
            case GL_BOOL_VEC2:
                glUniform2iv(uniform.location, count, (GLint*)value);
                break;

            case GL_INT_VEC3:
            case GL_BOOL_VEC3:
                glUniform3iv(uniform.location, count, (GLint*)value);
                break;

            case GL_INT_VEC4:
            case GL_BOOL_VEC4:
                glUniform4iv(uniform.location, count, (GLint*)value);
                break;

            case GL_FLOAT:
                glUniform1fv(uniform.location, count, (GLfloat*)value);
                break;

            case GL_FLOAT_VEC2:
                glUniform2fv(uniform.location, count, (GLfloat*)value);
                break;

            case GL_FLOAT_VEC3:
                glUniform3fv(uniform.location, count, (GLfloat*)value);
                break;

            case GL_FLOAT_VEC4:
                glUniform4fv(uniform.location, count, (GLfloat*)value);
                break;

            case GL_FLOAT_MAT2:
                glUniformMatrix2fv(uniform.location, count, transpose, (GLfloat*)value);
                break;

            case GL_FLOAT_MAT3:
                glUniformMatrix3fv(uniform.location, count, transpose, (GLfloat*)value);
                break;

            case GL_FLOAT_MAT4:
                glUniformMatrix4fv(uniform.location, count, transpose, (GLfloat*)value);
                break;

            default:
                // If the type is not supported, log this message
                Core::Debug::Log::error("Type not supported");
                break;
        }
        #pragma endregion
    }

    void ShaderProgram::bind() const
    {
        glUseProgram(programID);
    }

    void ShaderProgram::unbind() const
    {
        glUseProgram(0);
    }

    std::string ShaderProgram::getName()
    {
        return name;
    }
}