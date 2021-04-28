#pragma once

#include "singleton.hpp"

#include <vector>
#include <memory>

#include "renderer.hpp"

namespace LowRenderer
{
	class RenderManager final : public Singleton<RenderManager>
	{
		friend class Singleton<RenderManager>;

	private:
		RenderManager();
		~RenderManager();

		std::vector<std::shared_ptr<Renderer>> renderers;

	public:

		static void draw();

		static void addRenderer(const std::shared_ptr<Renderer>& rendererToDraw);
		static void clearRenderers();
	};
}