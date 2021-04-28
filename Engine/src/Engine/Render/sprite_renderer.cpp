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
		Core::Debug::Log::info("Draw " + lol);
	}

	void SpriteRenderer::update()
	{
		int val = std::stoi(lol);
		lol = std::to_string(val + 1);
	}
}