#pragma once

#include <bitset>

#include "maths.hpp"

#include "component.hpp"

#include "entity.hpp"

#include "model.hpp"
#include "property_extension.hpp"

namespace Physics
{
	using namespace Core::Maths;

	struct LowTransformContent
	{
		mat4 globalModelMatValue;
		mat4 modelMatValue;

		mat4 posMatValue;
		mat4 rotMatValue;
		mat4 scaleMatValue;

		vec3 posVecValue;
		vec3 rotVecValue;
		vec3 scaleVecValue;
	};

	struct TransformContent : LowTransformContent
	{
		TransformComponent* parent = nullptr;
		std::vector<TransformComponent*> children;
	};

	class Transform : protected TransformContent
	{
	protected:
		Transform() = default;

		void setChildrenDirty();

		enum dirtyFlag
		{
			GLOBAL_MODEL = 1 << 1,
			LOCAL_MODEL = 1 << 2,

			TRANSLATION_MAT = 1 << 3,
			ROTATION_MAT = 1 << 4,
			SCALE_MAT = 1 << 5,
		};

		void dirty(char toSetDirty)
		{
			dirtyBits |= toSetDirty;
		}

	private:
		char dirtyBits = 0;

		bool isDirty(char checkDirtyState = 0) const
		{
			return (dirtyBits & checkDirtyState) == checkDirtyState;
		}

		void clean(char toClean)
		{
			dirtyBits &= ~toClean;
		}

		bool isDirtyThenClean(char toClean = 0)
		{
			if (!isDirty(toClean))
				return false;

			clean(toClean);
			return true;
		}

		void setGlobalModelMat(const mat4& other)
		{
			globalModelMatValue = other;
		}

		mat4 getGlobalModelMat()
		{
			if (isDirtyThenClean(dirtyFlag::GLOBAL_MODEL))
			{
				globalModelMatValue = getParentModel() * model;

				setChildrenDirty();
			}

			return globalModelMatValue;
		}

		void setModelMat(const mat4& other)
		{
			modelMatValue = other;
			dirty(dirtyFlag::GLOBAL_MODEL);
		}

		mat4 getModelMat()
		{
			if (isDirtyThenClean(dirtyFlag::LOCAL_MODEL))
				modelMatValue = positionMat * rotationMat * scaleMat;

			return modelMatValue;
		}

		void setPosMat(const mat4& other)
		{
			posMatValue = other;
			dirty(dirtyFlag::LOCAL_MODEL | dirtyFlag::GLOBAL_MODEL);
		}

		mat4 getPosMat()
		{
			if (isDirtyThenClean(dirtyFlag::TRANSLATION_MAT))
				posMatValue = Core::Maths::translate(position);

			return posMatValue;
		}

		void setRotMat(const mat4& other)
		{
			rotMatValue = other;
			dirty(dirtyFlag::ROTATION_MAT | dirtyFlag::LOCAL_MODEL | dirtyFlag::GLOBAL_MODEL);
		}

		mat4 getRotMat()
		{
			if (isDirtyThenClean(dirtyFlag::ROTATION_MAT))
				rotMatValue = Core::Maths::rotateZ(rotVecValue.z) * Core::Maths::rotateY(rotVecValue.y) * Core::Maths::rotateX(rotVecValue.x);

			return rotMatValue;
		}

		void setScaleMat(const mat4& other)
		{
			scaleMatValue = other;
			dirty(dirtyFlag::SCALE_MAT | dirtyFlag::LOCAL_MODEL | dirtyFlag::GLOBAL_MODEL);
		}

		mat4 getScaleMat()
		{
			if (isDirtyThenClean(dirtyFlag::SCALE_MAT))
				scaleMatValue = Core::Maths::scale(scaleVecValue);

			return scaleMatValue;
		}

		void setPosVec(const vec3& other)
		{
			posVecValue = other;
			dirty(dirtyFlag::TRANSLATION_MAT | dirtyFlag::LOCAL_MODEL | dirtyFlag::GLOBAL_MODEL);
		}

		void setRotVec(const vec3& other)
		{
			rotVecValue = other;
			dirty(dirtyFlag::ROTATION_MAT | dirtyFlag::LOCAL_MODEL | dirtyFlag::GLOBAL_MODEL);
		}

		void setScaleVec(const vec3& other)
		{
			scaleVecValue = other;
			dirty(dirtyFlag::SCALE_MAT | dirtyFlag::LOCAL_MODEL | dirtyFlag::GLOBAL_MODEL);
		}

		vec3 getPosVec() { return posVecValue; }
		vec3 getRotVec() { return rotVecValue; }
		vec3 getScaleVec() { return scaleVecValue; }

		template<typename T>
		using transformProperty = propertyWrapper<Transform, T>;

		using mat4Property = transformProperty<mat4>;
		using vec3Property = transformProperty<vec3>;

	public:
		mat4 getGlobalModel() const;
		mat4 getParentModel() const;

		vec3Property position{ this, &Transform::setPosVec, &Transform::getPosVec };
		vec3Property rotation{ this, &Transform::setRotVec, &Transform::getRotVec };
		vec3Property scale{ this, &Transform::setScaleVec, &Transform::getScaleVec };

		mat4Property positionMat{ this, &Transform::setPosMat, &Transform::getPosMat };
		mat4Property rotationMat{ this, &Transform::setRotMat, &Transform::getRotMat };
		mat4Property scaleMat{ this, &Transform::setScaleMat, &Transform::getScaleMat };

		mat4Property model{ this, &Transform::setModelMat, &Transform::getModelMat };
		mat4Property globalModel{ this, &Transform::setGlobalModelMat, &Transform::getGlobalModelMat };
	};

	class TransformComponent : public Engine::Component, public Transform
	{
	private:
		void onDestroy() override;
		void removeChild(TransformComponent* child);
		void removeFromParent();

	public:
		TransformComponent(Engine::Entity& owner);

		Engine::Entity& getEntityParent();
		Physics::TransformComponent* getParent();
		TransformComponent* getChild(int childIndex);
		Engine::Entity& getEntityChild(int childIndex);
		int getChildrenCount();

		vec3 getGlobalRotation() const;
		vec3 getGlobalPosition() const;
		vec3 getParentRotation() const;
		vec3 getParentPosition() const;

		vec3 getForward();

		vec3 getPosition() const;
		vec3 getRotation() const;
		vec3 getScale() const;

		bool hasParent();
		bool hasChild();
		void setParent(Engine::Entity& newParent);
		void setParent(Physics::TransformComponent* newParent);
		void setChild(Physics::TransformComponent* child);
		void setChild(Engine::Entity& newParent);

		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::Entity& entity, std::istringstream& iss, std::string& parentName);
	};
}