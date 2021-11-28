#include "transform.hpp"

#include "imgui.h"

#include "utils.hpp"

namespace Physics
{
	void Transform::setChildrenDirty()
	{
		for (auto child : children)
			child->dirty(dirtyFlag::GLOBAL_MODEL);
	}

	Core::Maths::mat4 Transform::getGlobalModel() const
	{
		return globalModel;
	}

	Core::Maths::mat4 Transform::getParentModel() const
	{
		if (parent)
			return parent->globalModel;

		return Core::Maths::identity();
	}

	TransformComponent::TransformComponent(Engine::Entity& owner)
		: Component(owner)
	{

	}

	void TransformComponent::onDestroy()
	{
		removeFromParent();
	}

	Core::Maths::vec3 TransformComponent::getPosition() const
	{
		return position;
	}

	Core::Maths::vec3 TransformComponent::getRotation() const
	{
		return rotation;
	}

	Core::Maths::vec3 TransformComponent::getScale() const
	{
		return scale;
	}

	bool TransformComponent::hasParent()
	{
		return parent != nullptr;
	}

	bool TransformComponent::hasChild()
	{
		return children.size() > 0;
	}

	TransformComponent* TransformComponent::getChild(int childIndex)
	{
		return children[childIndex];
	}

	Engine::Entity& TransformComponent::getEntityChild(int childIndex)
	{
		return children[childIndex]->getHost();
	}

	int TransformComponent::getChildrenCount()
	{
		return (int)(children.size());
	}

	Engine::Entity& TransformComponent::getEntityParent()
	{
		return parent->getHost();
	}

	Physics::TransformComponent* TransformComponent::getParent()
	{
		return parent;
	}

	void TransformComponent::removeFromParent()
	{
		if (hasParent())
			getParent()->removeChild(this);
	}

	void TransformComponent::removeChild(TransformComponent* child)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i] != child)
				continue;

			children[i] = children.back();
			children.pop_back();

			return;
		}
	}

	Core::Maths::vec3 TransformComponent::getGlobalRotation() const
	{
		return rotation + getParentRotation();
	}

	Core::Maths::vec3 TransformComponent::getGlobalPosition() const
	{
		return position + getParentPosition();
	}

	Core::Maths::vec3 TransformComponent::getParentRotation() const
	{
		if (parent)
			return parent->getGlobalRotation();

		return Core::Maths::vec3(0.f, 0.f, 0.f);
	}

	Core::Maths::vec3 TransformComponent::getParentPosition() const
	{
		if (parent)
			return parent->getGlobalPosition();

		return Core::Maths::vec3(0.f, 0.f, 0.f);
	}

	Core::Maths::vec3 TransformComponent::getForward()
	{
		auto model = getGlobalModel();

		return -Core::Maths::vec3(model.e[2], model.e[6], model.e[10]);
	}

	void TransformComponent::setParent(Physics::TransformComponent* newParent)
	{
		if (parent == newParent)
			return;

		removeFromParent();

		parent = newParent;
		dirty(dirtyFlag::GLOBAL_MODEL);
	}

	void TransformComponent::setParent(Engine::Entity& owner)
	{
		TransformComponent* newParent;
		if (!owner.tryGetComponent<TransformComponent>(newParent))
			return;

		setParent(newParent);
	}

	void TransformComponent::setChild(Physics::TransformComponent* child)
	{
		children.push_back(child);
	}

	void TransformComponent::setChild(Engine::Entity& owner)
	{
		TransformComponent* newParent;
		if (!owner.tryGetComponent<TransformComponent>(newParent))
			return;

		children.push_back(newParent);
	}

	void TransformComponent::drawImGui()
	{
		if (ImGui::TreeNode("Transform"))
		{
			Core::Maths::vec3 tempPosition = position;
			Core::Maths::vec3 tempRotation = (Core::Maths::vec3)rotation * Core::Maths::RAD2DEG;
			Core::Maths::vec3 tempScale = scale;

			if (ImGui::DragFloat3("Position :", &tempPosition.x))
				position = tempPosition;

			if (ImGui::DragFloat3("Rotation :", &tempRotation.x, 1.f, 0.f, 360.f))
				rotation = tempRotation * Core::Maths::DEG2RAD;

			if (ImGui::DragFloat3("Scale :", &tempScale.x))
				scale = tempScale;

			if (parent)
			{
				std::string parentName = "Parent: " + parent->getHost().m_name;
				ImGui::Text(parentName.c_str());
			}

			Component::drawImGui();

			ImGui::TreePop();
		}
	}

	std::string TransformComponent::toString() const
	{
		return  "COMP TRANSFORM " + Utils::vecToStringParsing(position) +
									Utils::vecToStringParsing(rotation) +
									Utils::vecToStringParsing(scale) + (parent ? parent->getHost().m_name : "none");
	}

	void TransformComponent::parseComponent(Engine::Entity& owner, std::istringstream& iss, std::string& parentName)
	{
		TransformComponent* transform;
		if (!owner.tryGetComponent(transform))
			transform = owner.addComponent<TransformComponent>();

		Core::Maths::vec3 position;
		iss >> position.x;
		iss >> position.y;
		iss >> position.z;
		transform->position = position;

		Core::Maths::vec3 rotation;
		iss >> rotation.x;
		iss >> rotation.y;
		iss >> rotation.z;
		transform->rotation = rotation;

		Core::Maths::vec3 scale;
		iss >> scale.x;
		iss >> scale.y;
		iss >> scale.z;
		transform->scale = scale;

		iss >> parentName;
	}
}
