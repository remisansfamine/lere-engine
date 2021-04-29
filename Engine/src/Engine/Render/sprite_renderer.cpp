#include "sprite_renderer.hpp"

#include "debug.hpp"

#include "render_manager.hpp"


namespace LowRenderer
{
	SpriteRenderer::SpriteRenderer(Engine::GameObject& gameObject)
		: Renderer(gameObject, std::shared_ptr<SpriteRenderer>(this))
	{

	}

	void SpriteRenderer::draw()
	{
	}

	void SpriteRenderer::update()
	{
	}
}