#include "sprite_renderer.hpp"

#include "debug.hpp"

#include "render_manager.hpp"

#include "inputs_manager.hpp"
#include "time.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	SpriteRenderer::SpriteRenderer(Engine::GameObject& gameObject)
		: Renderer(gameObject, std::shared_ptr<SpriteRenderer>(this))
	{ }

	void SpriteRenderer::draw()
	{
		//Core::Debug::Log::info("Et moi un Sprite Renderer :P");
	}

	void SpriteRenderer::update()
	{
		m_transform->m_position.x += Core::Input::InputManager::getAxis("Horizontal") * Core::TimeManager::getDeltaTime();
	}
}