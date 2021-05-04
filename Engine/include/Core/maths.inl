
namespace Core::Maths
{
	float vec3::squaredMagnitude() const
	{
		return dot(*this, *this);
	}

	float vec3::magnitude() const
	{
		return sqrtf(squaredMagnitude());
	}

	vec3 vec3::normalized() const
    {
		float length = magnitude();

		if (length == 0.f)
			return *this;

		return *this / length;
	}

    vec3& vec3::normalize()
	{
		*this = this->normalized();

		return *this;
	}
}
