
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

	vec3 vectorRotate(const vec3& v, const quat& q)
	{
		return {
			v.x * (q.x * q.x + q.w * q.w - q.y * q.y - q.z * q.z) + v.y * (2 * q.x * q.y - 2 * q.w * q.z) + v.z * (2 * q.x * q.z + 2 * q.w * q.y),
			v.x * (2 * q.w * q.z + 2 * q.x * q.y) + v.y * (q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z) + v.z * (-2 * q.w * q.x + 2 * q.y * q.z),
			v.x * (-2 * q.w * q.y + 2 * q.x * q.z) + v.y * (2 * q.w * q.x + 2 * q.y * q.z) + v.z * (q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z)
		};
	}

	void quat::getAxisAngle(vec3& axis, float& angle) const
	{
		quat normalizedQuat = *this;

		if (fabs(w) > 1.0f)
			normalizedQuat = normalized();

		vec3 resAxis;
		float den = sqrtf(1.0f - w * w);

		if (den > 0.0001f)
			axis = vec3(x, y, z) / den;	// Axis
		else
			axis.x = 1.0f;		// Default axis

		angle = 2.0f * acosf(w);
	}

	float quat::length() const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	quat quat::normalized() const
	{
		float _length = length();

		if (_length == 0.0f)
			return *this;

		float invLength = 1.0f / _length;

		return {
			x * invLength,
			y * invLength,
			z * invLength,
			w * invLength
		};
	}

	quat quat::identity()
	{
		return { 0.f, 0.f, 0.f, 1.f };
	}
}
