#include "transform.hpp"

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

	/*Core::Maths::mat4 Transform::getPushModel()
	{
		mat4 pushModel = mat4x4::identity();
		Physics::Transform* curParent = parent;

		if (curParent != nullptr)
			pushModel = pushModel * curParent->getPushModel();

		pushModel = pushModel * getModel();

		return pushModel;
	}*/
}