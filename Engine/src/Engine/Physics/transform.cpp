#include "transform.hpp"

#include "imgui.h"

#include "utils.hpp"

namespace Physics
{
	Transform::Transform(Engine::GameObject& gameObject)
		: Transform(gameObject, std::shared_ptr<Transform>(this))
	{

	}

	Transform::Transform(Engine::GameObject& gameObject, const std::shared_ptr<Transform>& ptr)
		: Component(gameObject, ptr)
	{
	}

	Core::Maths::mat4 Transform::getModel()
	{
		// Return the model matrix (TRS where R = ZXY like Unity)
		return Core::Maths::translate(m_position) *
			   Core::Maths::rotateZ(m_rotation.z) *
			   Core::Maths::rotateX(m_rotation.x) *
			   Core::Maths::rotateY(m_rotation.y) *
			   Core::Maths::scale(m_scale);
	}

	Core::Maths::mat4 Transform::getGlobalModel()
	{
		if (parent)
			return getParentModel() * getModel();
			//return getModel() * getParentModel();

		return getModel();
	}

	Core::Maths::mat4 Transform::getParentModel()
	{
		if (parent)
			return parent->getGlobalModel();

		return Core::Maths::identity();
	}

	void Transform::setParent(std::shared_ptr<Physics::Transform> _parent)
	{
		parent = _parent;
	}

	void Transform::setParent(Engine::GameObject& gameObject)
	{
		std::shared_ptr<Transform> newParent;
		if (!gameObject.tryGetComponent<Transform>(newParent))
			return;

		parent = newParent;
	}

	void Transform::drawImGui()
	{
		if (ImGui::TreeNode("Transform"))
		{
			Core::Maths::vec3 rotateDegrees = m_rotation * Core::Maths::RAD2DEG;

			ImGui::DragFloat3("Position :", &m_position.x);
			ImGui::DragFloat3("Rotation :", &rotateDegrees.x, 1.f, 0.f, 360.f);
			ImGui::DragFloat3("Scale :", &m_scale.x);

			m_rotation = rotateDegrees * Core::Maths::DEG2RAD;

			/*if (parent)
			{
				std::string parentName = "Parent: " + getHost().m_name;
				ImGui::Text(parentName.c_str());
			}*/


			ImGui::TreePop();
		}
	}

	std::string Transform::toString()
	{
		return  "position = " + Utils::vecToString(m_position) +
				"rotation = " + Utils::vecToString(m_rotation) +
				"scale = " + Utils::vecToString(m_scale);
	}
}