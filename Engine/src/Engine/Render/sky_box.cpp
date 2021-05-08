#include "sky_box.hpp"

#include "resources_manager.hpp"
#include "render_manager.hpp"
#include "render_manager.hpp"

namespace LowRenderer
{
	SkyBox::SkyBox(Engine::GameObject& gameObject, const std::string& shaderPromgramName, std::shared_ptr<SkyBox> ptr)
		: Component(gameObject, ptr)
	{
		cubeMesh		= Resources::ResourcesManager::getMeshByName("cube");
		m_shaderProgram = Resources::ResourcesManager::loadShaderProgram(shaderPromgramName);
		LowRenderer::RenderManager::linkComponent(ptr);
	}

	SkyBox::SkyBox(Engine::GameObject& gameObject, const std::vector<std::string>& paths, const std::string& shaderPromgramName)
		: SkyBox(gameObject, shaderPromgramName, std::shared_ptr<SkyBox>(this))
	{
		// Delegation block other members initialization
		cubeMap = Resources::ResourcesManager::loadCubeMap(paths);
	}

	SkyBox::~SkyBox()
	{
		//Core::Debug::Log::info("Unload model " + cubeMap;
	}

	void SkyBox::draw() const
	{
		glDepthMask(0);

		m_shaderProgram->bind();

		std::shared_ptr<Camera> cam = LowRenderer::RenderManager::getCurrentCamera();

		Core::Maths::mat4 newView =  Core::Maths::toMat4(Core::Maths::toMat3(cam->getViewMatrix()));
		m_shaderProgram->setUniform("viewProj", (cam->getProjection() * newView).e, 1, 1);

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->getID());
		
		cubeMesh->draw();

		m_shaderProgram->unbind();

		glDepthMask(1);
	}
}