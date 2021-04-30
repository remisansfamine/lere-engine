#include "resources_manager.hpp"

#include "debug.hpp"

namespace Resources
{
	ResourcesManager::ResourcesManager()
	{
		Core::Debug::Log::info("Creating the Resources Manager");

	}

	ResourcesManager::~ResourcesManager()
	{
		Core::Debug::Log::info("Destroying the Resources Manager");
	}

	void ResourcesManager::init()
	{
		ResourcesManager* RM = instance();

		if (RM->initialized)
		{
			Core::Debug::Log::error("The Resources Manager is already initialized");
			return;
		}

		// TODO : Add basic materials/models/textures

		RM->initialized = true;

		Core::Debug::Log::info("Resources Manager initialized");
	}

	std::shared_ptr<Scene> ResourcesManager::loadScene(const std::string& scenePath)
	{
		ResourcesManager* RM = instance();
		
		const auto& sceneIt = RM->scenes.find(scenePath);

		if (sceneIt != RM->scenes.end())
		{
			return sceneIt->second;
		}

		// TODO: Add Warn load fail
		return RM->scenes[scenePath] = std::make_shared<Scene>(Scene(scenePath));
	}

	std::shared_ptr<Shader> ResourcesManager::loadShader(const std::string& shaderPath)
	{
		ResourcesManager* RM = instance();
		
		const auto& shaderIt = RM->shaders.find(shaderPath);

		if (shaderIt != RM->shaders.end())
		{
			return shaderIt->second;
		}

		// TODO: Add Warn load fail
		return RM->shaders[shaderPath] = std::make_shared<Shader>(Shader(shaderPath));
	}

	std::shared_ptr<ShaderProgram> ResourcesManager::loadShaderProgram(const std::string& programName, const std::string& vertPath, const std::string& fragPath)
	{
		ResourcesManager* RM = instance();

		const auto& programIt = RM->shaderPrograms.find(programName);

		if (programIt != RM->shaderPrograms.end())
		{
			return programIt->second;
		}

		// TODO: Add Warn load fail
		return RM->shaderPrograms[programName] = std::make_shared<ShaderProgram>(ShaderProgram(programName, vertPath, fragPath));
	}

	std::shared_ptr<Texture> ResourcesManager::loadTexture(const std::string& texturePath)
	{
		ResourcesManager* RM = instance();

		const auto& textureIt = RM->textures.find(texturePath);

		if (textureIt != RM->textures.end())
		{
			return textureIt->second;
		}

		// TODO: Add Warn load fail
		return RM->textures[texturePath] = std::make_shared<Texture>(Texture(texturePath));
	}

	std::shared_ptr<Material> ResourcesManager::loadMaterial(const std::string& materialPath)
	{
		ResourcesManager* RM = instance();

		const auto& materialIt = RM->materials.find(materialPath);

		if (materialIt != RM->materials.end())
		{
			return materialIt->second;
		}

		// TODO: Add Warn load fail
		return RM->materials[materialPath] = std::make_shared<Material>(Material());
	}
}