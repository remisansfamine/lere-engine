#include "sprite_renderer.hpp"

#include "debug.hpp"

#include "render_manager.hpp"


namespace LowRenderer
{
	SpriteRenderer::SpriteRenderer(Engine::GameObject& gameObject)
		: Renderer(gameObject, std::shared_ptr<SpriteRenderer>(this))
	{
		//requireComponent<Light>();

	}

	void SpriteRenderer::draw()
	{
		Core::Debug::Log::info("Et moi un Sprite Renderer :P");
	}

	void SpriteRenderer::update()
	{
	}
}