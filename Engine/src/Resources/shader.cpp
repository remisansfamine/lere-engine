#include "shader.hpp"

#include <fstream>
#include <filesystem>

#include <imgui.h>

#include "resources_manager.hpp"
#include "render_manager.hpp"
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

        load();
        //ResourcesManager::manageTask(&Shader::setCode, this);
	}

    Shader::~Shader()
    {
        destroy();
    }

    void Shader::load()
    {
        setID();
        create();
        setCode();
    }

    void Shader::reload()
    {
        destroy();
        create();
        setCode();
    }

    bool Shader::setID()
    {
        // Check if the file can be read
        if (!std::filesystem::exists(m_filePath))
        {
            Core::Debug::Log::error("Cannot open the file " + m_filePath);
            return false;
        }

        // Create the shader by checking its extension
        if (Utils::hasSuffix(m_filePath, ".vert"))
        {
            type = GL_VERTEX_SHADER;
            return true;
        }

        if (Utils::hasSuffix(m_filePath, ".frag"))
        {
            type = GL_FRAGMENT_SHADER;
            return true;
        }

        if (Utils::hasSuffix(m_filePath, ".geom"))
        {
            type = GL_GEOMETRY_SHADER;
            return true;
        }

        Core::Debug::Log::error("File extension is not compatible");

        return false;
    }

    void Shader::setCode()
    {
        std::ifstream ifs(ResourcesManager::getResourcesPath() + m_filePath);

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

    void Shader::create()
    {
        shaderID = glCreateShader(type);
    }

    void Shader::destroy()
    {
        glDeleteShader(shaderID);
        shaderID = 0;
    }

    void Shader::drawImGui()
    {
        if (ImGui::TreeNode(m_filePath.c_str()))
        {
            ImGui::Text("ID: %i", &shaderID);

            ImGui::TreePop();
        }
    }

#pragma endregion


#pragma region SHADER_PROGRAM
	ShaderProgram::ShaderProgram(const std::string& programName, const std::string& vertPath, const std::string& fragPath, const std::string& geomPath)
        : name(programName)
	{
        Core::Debug::Log::info("Loading program " + programName);

        Core::Debug::Log::info("Linking " + vertPath + " and " + fragPath + " to " + programName);

        create();

        vertShader = Resources::ResourcesManager::loadShader(vertPath);
        fragShader = Resources::ResourcesManager::loadShader(fragPath);

        if (geomPath != "")
            geomShader = Resources::ResourcesManager::loadShader(geomPath);

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


        // TODO: REMOVE THIS SHIT 
        LowRenderer::RenderManager::bindUBO(this, "lightBlock");
    }

    void ShaderProgram::create()
    {
        programID = glCreateProgram();
    }

    void ShaderProgram::destroy()
    {
        glDeleteProgram(programID);
        programID = 0;
    }

    ShaderProgram::~ShaderProgram()
    {
        destroy();
    }

    void ShaderProgram::loadUniforms()
    {
        uniforms.clear();
        samplerIDs.clear();

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

            switch (type)
            {
            case GL_SAMPLER_2D:
                samplerIDs[uniName] = { (GLuint)samplerIDs.size(), GL_TEXTURE_2D };
                break;
            case GL_SAMPLER_CUBE:
                samplerIDs[uniName] = { (GLuint)samplerIDs.size(), GL_TEXTURE_CUBE_MAP };
                break;
            }

            // Create a new uniform with the location and the type
            // And add it to a map
            uniforms[uniName] = LowRenderer::Uniform(location, type);
        }
    }

    void ShaderProgram::loadLocations()
    {
        loadUniforms();
    }

    void ShaderProgram::setUniform(const std::string& target, const void* value, bool shouldBeTracked, int count, bool transpose) const
    {
        if (!value)
        {
            Core::Debug::Log::error("Value sent at " + target + " is not valid");
            return;
        }

        // Get the iterator with the target
        const auto& currentIt = uniforms.find(target);

        // Check if this iterator is valid
        if (currentIt == uniforms.end())
        {
            if (debugSetUniform)
                Core::Debug::Log::warning("Cannot find the uniform named: " + target + " - There is no uniform named like that");

            return;
        }

        const LowRenderer::Uniform& uniform = currentIt->second;

        if (!uniform.bind(value, count, transpose))
            Core::Debug::Log::error("Cannot find the uniform named: " + target);
    }

    void Sampler::bind(GLuint textureID)
    {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(type, textureID);
    }

    bool ShaderProgram::setSampler(const std::string& target, GLuint textureID)
    {
        if (!textureID)
            return false;

        const auto& samplerIt = samplerIDs.find(target);

        if (samplerIt == samplerIDs.end())
            return false;

        auto [samplerName, sampler] = *samplerIt;

        sampler.bind(textureID);

        return true;
    }

    bool ShaderProgram::bind() const
    {
        if (!programID)
            return false;

        glUseProgram(programID);

        for (auto [samplerName, ID] : samplerIDs)
            setUniform(samplerName, &ID, false);

        return true;
    }

    void ShaderProgram::unbind() const
    {
        glUseProgram(0);
    }

    void ShaderProgram::reload()
    {
        Core::Debug::Log::info("Reloading shader program " + name);

        destroy();

        if (vertShader)
            vertShader->reload();

        if (fragShader)
            fragShader->reload();

        if (geomShader)
            geomShader->reload();

        create();

        ResourcesManager::addToMainThreadInitializerQueue(this);
    }

    std::string ShaderProgram::getName()
    {
        return name;
    }

    bool ShaderProgram::bindToUBO(const std::string& UBOName, GLint UBOBindID)
    {
        GLint UBOIndex = glGetUniformBlockIndex(programID, UBOName.c_str());

        if (UBOIndex < 0)
            return false;

        glUniformBlockBinding(programID, UBOIndex, UBOBindID);
        return true;
    }

    void ShaderProgram::drawImGui()
    {
        if (ImGui::TreeNode(name.c_str()))
        {
            ImGui::Checkbox("Debug set uniform", &debugSetUniform);

            ImGui::Text("Uniform count: %i", uniforms.size());

            if (vertShader)
                vertShader->drawImGui();

            if (fragShader)
                fragShader->drawImGui();

            if (geomShader)
                geomShader->drawImGui();

            if (ImGui::Button("Reload"))
                reload();

            ImGui::TreePop();
        }
    }
}