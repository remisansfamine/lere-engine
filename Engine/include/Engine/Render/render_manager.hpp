#pragma once

#include "singleton.hpp"

#include <vector>
#include <memory>

#include "renderer.hpp"
#include "light.hpp"
#include "camera.hpp"
#include "sky_box.hpp"
#include "collider_renderer.hpp"

namespace LowRenderer
{
	class RenderManager final : public Singleton<RenderManager>
	{
		friend class Singleton<RenderManager>;

	private:
		RenderManager();
		~RenderManager();

		std::vector<std::shared_ptr<ColliderRenderer>> colliders;
		std::vector<std::shared_ptr<Renderer>> renderers;
		std::vector<std::shared_ptr<Light>> lights;
		std::vector<std::shared_ptr<Camera>> cameras;
		std::vector<std::shared_ptr<SkyBox>> skyBoxes;

		void drawColliders() const;

	public:

		static std::shared_ptr<Camera> getCurrentCamera();

		static void draw();

		static void linkComponent(const std::shared_ptr<Light>& compToLink);

		static void linkComponent(const std::shared_ptr<Renderer>& compToLink);

		static void linkComponent(const std::shared_ptr<Camera>& compToLink);

		static void linkComponent(const std::shared_ptr<SkyBox>& compToLink);

		static void linkComponent(const std::shared_ptr<ColliderRenderer>& compToLink);

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

		template<>
		static void clearComponents<Camera>()
		{
			instance()->cameras.clear();
		}

		template<>
		static void clearComponents<SkyBox>()
		{
			instance()->skyBoxes.clear();
		}

		template<>
		static void clearComponents<ColliderRenderer>()
		{
			instance()->colliders.clear();
		}
	};
}