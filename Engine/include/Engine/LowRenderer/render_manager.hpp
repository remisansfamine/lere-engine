#pragma once

#include "singleton.hpp"

#include <glad\glad.h>

#include <unordered_set>
#include <set>

#include "collider_renderer.hpp"
#include "sprite_renderer.hpp"
#include "model_renderer.hpp"
#include "sky_box.hpp"
#include "camera.hpp"
#include "light.hpp"

namespace LowRenderer
{
	class RenderManager final : public Singleton<RenderManager>
	{
		friend class Singleton<RenderManager>;

	private:
		RenderManager();
		~RenderManager();

		std::unordered_map<GLenum, bool> enabledCaps;
		std::unordered_map<std::string, std::shared_ptr<UniformBlock>> uniformBlocks;

		std::unordered_set<ColliderRenderer*> colliders;
		std::unordered_set<SpriteRenderer*> sprites;
		std::unordered_set<ModelRenderer*> models;
		std::unordered_set<SkyBox*> skyBoxes;
		std::unordered_set<Light*> lights;
		std::set<Camera*> cameras;

		void drawColliders() const;

		void drawShadows();
		void drawSkybox();
		void drawModels();
		void drawSprites();

		template <class C>
		static void clearComponents();

		template<>
		static void clearComponents<Light>()
		{
			instance()->lights.clear();
		}

		template<>
		static void clearComponents<SpriteRenderer>()
		{
			instance()->sprites.clear();
		}

		template<>
		static void clearComponents<ModelRenderer>()
		{
			instance()->models.clear();
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

	public:
		static void GLSetCapState(const GLenum cap, bool state);
		static void GLEnable(const GLenum cap);
		static void GLDisable(const GLenum cap);

		static Camera* getCurrentCamera();

		static void draw();

		static void linkComponent(Light* compToLink);
		static void linkComponent(ModelRenderer* compToLink);
		static void linkComponent(SpriteRenderer* compToLink);
		static void linkComponent(Camera* compToLink);
		static void linkComponent(ColliderRenderer* compToLink);
		static void linkComponent(SkyBox* compToLink);

		static void removeComponent(ColliderRenderer* compToRemove);
		static void removeComponent(SpriteRenderer* compToRemove);
		static void removeComponent(ModelRenderer* compToRemove);

		static void clearAll();

		static void bindUBO(Resources::ShaderProgram* program, const std::string& UBOName);
	};
}