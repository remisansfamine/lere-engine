#include "light.hpp"

#include "imgui.h"

#include "render_manager.hpp"
#include "resources_manager.hpp"
#include "inputs_manager.hpp"
#include "time.hpp"

#include "transform.hpp"
#include "shadow_point.hpp"
#include "shadow_map.hpp"
#include "utils.hpp"

namespace LowRenderer
{
	Light::Light(Engine::Entity& owner)
		: Component(owner)
	{
		m_transform = requireComponent<Physics::TransformComponent>();
		shadow = std::make_unique<ShadowMap>();

		LowRenderer::RenderManager::linkComponent(this);
	}

	void Light::setAsDirectionnal()
	{
		isPoint = 0.f;

		cutoff = Core::Maths::PI;
		outterCutoff = Core::Maths::PI;
	}

	void Light::setAsPoint()
	{
		isPoint = 1.f;

		cutoff = Core::Maths::PI;
		outterCutoff = Core::Maths::PI;
	}

	void Light::setAsSpot()
	{
		isPoint = 1.f;

		cutoff = Core::Maths::PIO4;
		outterCutoff = 50.f * Core::Maths::DEG2RAD;
	}

	void Light::setShadows(bool isShadow)
	{
		hasShadow = (float)isShadow;

		if (isShadow)
		{
			bool isShadowMap = dynamic_cast<ShadowMap*>(shadow.get()) != nullptr;

			if (!isShadowMap && isPoint == 0.0)
				shadow = std::make_unique<ShadowMap>();
			else if (!isShadowMap && dynamic_cast<ShadowPoint*>(shadow.get()) == nullptr)
				shadow = std::make_unique<ShadowPoint>();

			return;
		}

		shadow = nullptr;
	}

	void Light::compute()
	{
		enable = (float)isActive();
		hasShadow = (float)(shadow != nullptr);
		position = m_transform->position;

		if (hasShadow == 0.f || isPoint != 0.f)
			return;

		Core::Maths::mat4 lightView = Core::Maths::lookAt(m_transform->position);
		spaceMatrix = Core::Maths::orthographic(-90.f, 90.f, -90.f, 90.f, -90.f, 180.f) * lightView;
	}

	void Light::sendToProgram(std::shared_ptr<Resources::ShaderProgram> program, int index) const
	{
		// Send light parameters to the ShaderProgram packed into matrices
		program->setUniform("lightAttribs1[" + std::to_string(index) + "][0]", &position, true);
		program->setUniform("lightAttribs2[" + std::to_string(index) + "][0]", &attenuation, true);

		if (shadow != nullptr)
		{
			int correspondingIndex = index;
			if (isPoint == 0.f)
			{
				correspondingIndex += 6;
				program->setUniform("lightAttribs3[" + std::to_string(index) + "][0]", &spaceMatrix.e, true, 1, 1);
				program->setUniform("shadowMaps[" + std::to_string(index) + "][0]", &correspondingIndex, true);

				glActiveTexture(GL_TEXTURE0 + correspondingIndex);
				glBindTexture(GL_TEXTURE_2D, shadow->ID);
			}
			else
			{
				correspondingIndex += 14;
				float farPlane = 25.f;
				program->setUniform("farPlane", &farPlane, true);
				program->setUniform("shadowCubeMaps[" + std::to_string(index) + "][0]", &correspondingIndex, true);

				glActiveTexture(GL_TEXTURE0 + correspondingIndex);
				glBindTexture(GL_TEXTURE_CUBE_MAP, shadow->ID);
			}
		}
	}

	void Light::addToLightBuffer(std::vector<LightData>& buffer)
	{
		buffer.push_back({position,isPoint,ambient, diffuse ,specular ,attenuation ,cutoff ,direction ,outterCutoff ,enable ,hasShadow ,spaceMatrix });
	}

	const Core::Maths::mat4& Light::getSpaceMatrix() const
	{
		return spaceMatrix;
	}

	void Light::drawImGui()
	{
		if (ImGui::TreeNode("Light"))
		{
			bool isPointFlag = isPoint;
			if (ImGui::Checkbox("Is point light", &isPointFlag))
				isPoint = isPointFlag;

			ImGui::ColorEdit3("Ambient: ", &ambient.data.x);
			ImGui::ColorEdit3("Diffuse: ", &diffuse.data.x);
			ImGui::ColorEdit3("Specular: ", &specular.data.x);
			ImGui::DragFloat3("Attenuation: ", &attenuation.x);
			ImGui::DragFloat3("Direction: ", &direction.x);
			ImGui::DragFloat("Cutoff: ", &cutoff);
			ImGui::DragFloat("Outer cutoff: ", &outterCutoff);

			Component::drawImGui();

			ImGui::TreePop();
		}
	}

	std::string Light::toString() const
	{
		return "COMP LIGHT " + std::to_string(isPoint) + " " +
							   Utils::vecToStringParsing(ambient.data) +
							   Utils::vecToStringParsing(diffuse.data) +
							   Utils::vecToStringParsing(specular.data) +
							   Utils::vecToStringParsing(attenuation) +
							   std::to_string(cutoff) + " " +
							   Utils::vecToStringParsing(direction) +
							   std::to_string(outterCutoff) + " " +
							   std::to_string(enable) + " " + std::to_string(shadow == nullptr);
	}

	void Light::parseComponent(Engine::Entity& owner, std::istringstream& iss)
	{
		owner.addComponent<Light>();
		auto light = owner.getComponent<Light>();

		iss >> light->isPoint;

		iss >> light->ambient.data.r;
		iss >> light->ambient.data.g;
		iss >> light->ambient.data.b;
		iss >> light->ambient.data.a;

		iss >> light->diffuse.data.r;
		iss >> light->diffuse.data.g;
		iss >> light->diffuse.data.b;
		iss >> light->diffuse.data.a;

		iss >> light->specular.data.r;
		iss >> light->specular.data.g;
		iss >> light->specular.data.b;
		iss >> light->specular.data.a;

		iss >> light->attenuation.c;
		iss >> light->attenuation.l;
		iss >> light->attenuation.q;

		iss >> light->cutoff;

		iss >> light->direction.x;
		iss >> light->direction.y;
		iss >> light->direction.z;

		iss >> light->outterCutoff;

		iss >> light->enable;

		bool hasShadow = false;
		iss >> hasShadow;
		light->setShadows(hasShadow);
	}
}