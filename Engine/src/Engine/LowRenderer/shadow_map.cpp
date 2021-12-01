#include "shadow_map.hpp"

#include "resources_manager.hpp"
#include "render_manager.hpp"
#include "debug.hpp"

namespace LowRenderer
{
	ShadowMap::ShadowMap()
		: Shadow("depthShader")
	{
		Core::Debug::Log::info("Create ShadowMap");

		create();
	}

	void ShadowMap::generateBuffer()
	{
		glGenFramebuffers(1, &FBO);
	}

	void ShadowMap::generateTexture()
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		// Clamp to border for deactivate frustum shadows
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


		float borderColor[] = { 1.f, 1.f, 1.f, 1.f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void ShadowMap::attachTextureToBuffer()
	{
		Core::Debug::Log::info("ShadowMap : Attach texture and depth buffer");

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShadowMap::create()
	{
		generateBuffer();
		generateTexture();
		attachTextureToBuffer();
	}

	void ShadowMap::sendToShader(const LowRenderer::Light* light) const
	{
		program->setUniform("lightSpaceMatrix", light->getSpaceMatrix().e, false, 1, 1);
	}
}