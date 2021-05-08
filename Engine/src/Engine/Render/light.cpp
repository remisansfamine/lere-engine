#include "light.hpp"

#include "imgui.h"

#include "render_manager.hpp"
#include "inputs_manager.hpp"
#include "time.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	Light::Light(Engine::GameObject& gameObject)
		: Light(gameObject, std::shared_ptr<Light>(this))
	{
		m_transform = requireComponent<Physics::Transform>();
	}

	Light::Light(Engine::GameObject& gameObject, const std::shared_ptr<Light>& ptr)
		: Component(gameObject, ptr)
	{
		LowRenderer::RenderManager::linkComponent(ptr);
	}

	void Light::setAsDirectionnal()
	{
		position.w = 0.f;

		cutoff = Core::Maths::PI;
		outterCutoff = Core::Maths::PI;
	}

	void Light::setAsPoint()
	{
		position.w = 1.f;

		cutoff = Core::Maths::PI;
		outterCutoff = Core::Maths::PI;
	}

	void Light::setAsSpot()
	{
		position.w = 1.f;

		cutoff = Core::Maths::PIO4;
		outterCutoff = 50.f * Core::Maths::DEG2RAD;
	}

	void Light::compute()
	{
		enable = (float)isActive();
		position.xyz = m_transform->m_position;
	}

	void Light::sendToProgram(std::shared_ptr<Resources::ShaderProgram> program, int index) const
	{
		// Send light parameters to the ShaderProgram packed into matrices
		program->setUniform("lightAttribs1[" + std::to_string(index) + "][0]", &position);
		program->setUniform("lightAttribs2[" + std::to_string(index) + "][0]", &attenuation);
	}

	void Light::drawImGui()
	{
		if (ImGui::TreeNode("Light"))
		{
			ImGui::ColorEdit3("Ambient: ", &ambient.data.x);
			ImGui::ColorEdit3("Diffuse: ", &diffuse.data.x);
			ImGui::ColorEdit3("Specular: ", &specular.data.x);
			ImGui::DragFloat3("Attenuation: ", &attenuation.x);
			ImGui::DragFloat3("Direction: ", &direction.x);
			ImGui::DragFloat("Cutoff: ", &cutoff);
			ImGui::DragFloat("Outer cutoff: ", &outterCutoff);

			ImGui::TreePop();
		}
	}
}