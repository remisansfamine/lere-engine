#pragma once

#include "singleton.hpp"

#include <vector>
#include <memory>

#include "renderer.hpp"
#include "light.hpp"

namespace LowRenderer
{
	class RenderManager final : public Singleton<RenderManager>
	{
		friend class Singleton<RenderManager>;

	private:
		RenderManager();
		~RenderManager();

		std::vector<std::shared_ptr<Renderer>> renderers;
		std::vector<std::shared_ptr<Light>> lights;

	public:

		static void draw();

		static void linkComponent(const std::shared_ptr<Light>& compToLink);

		static void linkComponent(const std::shared_ptr<Renderer>& compToLink);

		template <class C>
		static void clearComponents();

		template<>
		static void clearComponents<Light>()
		{
			instance()->lights.clear();
		}

		template<>
		static void clearComponents<Renderer>()
		{
			instance()->renderers.clear();
		}
	};
}