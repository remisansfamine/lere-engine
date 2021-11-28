#pragma once

#include "property.hpp"

#include "maths.hpp"

template<typename OwnerT>
using floatProperty = propertyWrapper<OwnerT, float>;

template<typename OwnerT>
struct vec3Property : public propertyWrapper<OwnerT, Core::Maths::vec3>
{
    using super = propertyWrapper<OwnerT, Core::Maths::vec3>;

    OwnerT& operator+=(const  Core::Maths::vec3& value)
    {
        (super::ownerPtr->*super::setter)((super::ownerPtr->*super::getter) + value);
        return *super::ownerPtr;
    }

    OwnerT& operator-=(const  Core::Maths::vec3& value)
    {
        (super::ownerPtr->*super::setter)((super::ownerPtr->*super::getter) - value);
        return *super::ownerPtr;
    }
};