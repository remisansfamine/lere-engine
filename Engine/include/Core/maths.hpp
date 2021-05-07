#pragma once

#include <cmath>

namespace Core::Maths
{
    constexpr float TAU = 6.28318530717958647692f;
    constexpr float PI = 3.14159265358979323846f;
    constexpr float PIO4 = PI / 4.f;
    constexpr float PIO2  = PI / 2.f;

    constexpr float DEG2RAD = PI / 180.f;
    constexpr float RAD2DEG = 180.f / PI;

    union vec2
    {
        vec2(float x = 0.f, float y = 0.f)
            : x(x), y(y)
        {}

        float e[2];

        struct { float x; float y; };
        struct { float s; float t; };
        struct { float u; float v; };
    };

    union vec3
    {
        vec3(float x = 0.f, float y = 0.f, float z = 0.f)
            : x(x), y(y), z(z)
        {}
        vec3(vec2 xy, float z = 0.f)
            : x(xy.x), y(xy.y), z(z)
        {}

        float e[3];
        vec2  xy;
        vec2  rg;
        vec2  uv;

        struct { float x; float y; float z; };
        struct { float r; float g; float b; };
        struct { float c; float l; float q; };
        struct { float u; float v; float q; };

        inline float squaredMagnitude() const;

        inline float magnitude() const;

        inline vec3 normalized() const;

        inline vec3& normalize();
    };

    union vec4
    {
        vec4(float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f)
            : x(x), y(y), z(z), w(w)
        {}
        vec4(vec3 xyz, float w = 0.f)
            : x(xyz.x), y(xyz.y), z(xyz.z), w(w)
        {}
        vec4(vec2 xy, float z = 0.f, float w = 0.f)
            : x(xy.x), y(xy.y), z(z), w(w)
        {}

        float e[4];
        vec3  xyz;
        vec3  rgb;
        vec2  xy;
        vec2  rg;

        struct { float x; float y; float z; float w; };
        struct { float r; float g; float b; float a; };
    };

    union mat3
    {
        mat3() = default;
        float e[9] = { 0.f };
        vec3  c[3];
    };

    union mat4
    {
        mat4() = default;
        float e[16] = { 0.f };
        vec4  c[4];
    };

    union quat
    {
        quat(float x, float y, float z, float w)
            : x(x), y(y), z(z), w(w)
        { }

        quat(vec3 axis, float angle)
        {
            if (axis.magnitude() != 0.0f)
                angle *= 0.5f;

            axis = axis.normalized();

            float sinres = sinf(angle);
            float cosres = cosf(angle);

            *this = quat(axis.x * sinres, axis.y * sinres, axis.z * sinres, cosres).normalized();
        }

        float e[4];
        struct { float x; float y; float z; float w; };

        inline void getAxisAngle(vec3& axis, float& angle) const;

        inline float length() const;
        inline quat normalized() const;

        inline static quat identity();
    };

    inline mat4 identity()
    {
        return {
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
    }

    inline mat3 toMat3(const mat4& mat)
    {
        return {
            mat.e[0], mat.e[1], mat.e[2],
            mat.e[4], mat.e[5], mat.e[6],
            mat.e[8], mat.e[9], mat.e[10]
        };
    }

    inline mat4 translate(const vec3& value)
    {
        return {
            1.f, 0.f, 0.f, value.x,
            0.f, 1.f, 0.f, value.y,
            0.f, 0.f, 1.f, value.z,
            0.f, 0.f, 0.f, 1.f
        };
    }

    inline mat4 scale(const vec3& value)
    {
        return {
            value.x, 0.f, 0.f, 0.f,
            0.f, value.y, 0.f, 0.f,
            0.f, 0.f, value.z, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
    }

    inline mat4 rotateX(float angleRadians)
    {
        float cos = cosf(angleRadians), sin = sinf(angleRadians);

        return {
            1.f, 0.f, 0.f, 0.f,
            0.f, cos,-sin, 0.f,
            0.f, sin, cos, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
    }

    inline mat4 rotateY(float angleRadians)
    {
        float cos = cosf(angleRadians), sin = sinf(angleRadians);

        return {
            cos, 0.f, sin, 0.f,
            0.f, 1.f, 0.f, 0.f,
            -sin,0.f, cos, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
    }

    inline mat4 rotateZ(float angleRadians)
    {
        float cos = cosf(angleRadians), sin = sinf(angleRadians);

        return {
            cos, -sin, 0.f, 0.f,
            sin, cos, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
    }

    inline mat4 frustum(float left, float right, float bottom, float top, float near, float far)
    {
        // Pre-compute divisions
        float OneOverTopMinusBottom = 1.f / (top - bottom);
        float OneOverNeasMinusFar = 1.f / (near - far);
        float OneOverRightMinusLeft = 1.f / (right - left);

        return {
            2.f * near * OneOverRightMinusLeft, 0.f, (right + left) * OneOverRightMinusLeft, 0.f,
            0.f, 2.f * near * OneOverTopMinusBottom, (top + bottom) * OneOverTopMinusBottom, 0.f,
            0.f, 0.f, (far + near) * OneOverNeasMinusFar, 2.f * far * near * OneOverNeasMinusFar,
            0.f, 0.f, -1.f, 0.f
        };
    }

    inline mat4 orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        // Pre-compute divisions
        float OneOverTopMinusBottom = 1.f / (top - bottom);
        float OneOverFarMinusNear = 1.f / (near - far);
        float OneOverRightMinusLeft = 1.f / (right - left);

        return {
            2.f * OneOverRightMinusLeft, 0.f, 0.f, -(right + left) * OneOverRightMinusLeft,
            0.f, 2.f * OneOverTopMinusBottom, 0.f, -(top + bottom) * OneOverTopMinusBottom,
            0.f, 0.f, 2.f * OneOverFarMinusNear, (far + near) * OneOverFarMinusNear,
            0.f, 0.f, 0.f, 1.f
        };
    }

    inline mat4 perspective(float fovY, float aspect, float near, float far, bool isOrtho)
    {
        if (isOrtho)
        {
            // If the camera is orthographic, call this function
            float top = tanf(fovY * 0.5f);
            float right = top * aspect;

            return orthographic(-right, right, -top, top, near, far);
        }

        // Else call this one
        float top = near * tanf(fovY * 0.5f);
        float right = top * aspect;

        return frustum(-right, right, -top, top, near, far);
    }

    inline mat4 toMat4(const mat3& mat)
    {
        return {
            mat.e[0], mat.e[1], mat.e[2], 0.f,
            mat.e[3], mat.e[4], mat.e[5], 0.f,
            mat.e[6], mat.e[7], mat.e[8], 0.f,
              0.f,      0.f,      0.f,    1.f
        };
    }

    inline vec4 operator*(const mat4& m, const vec4& v)
    {
        vec4 result;

        for (int i = 0; i < 4; i++)
        {
            float sum = 0.f;
            for (int j = 0; j < 4; j++)
                sum += m.c[i].e[j] * v.e[j];

            result.e[i] = sum;
        }

        return result;
    }

    inline mat4 operator*(const mat4& a, const mat4& b)
    {
        mat4 result = { 0.f };

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                float sum = 0.f;
                for (int k = 0; k < 4; k++)
                    sum += a.c[i].e[k] * b.c[k].e[j];

                result.c[i].e[j] = sum;
            }
        }

        return result;
    }

    inline vec3 vectorRotate(const vec3& v, const quat& q);

    inline vec3 modelMatrixToPosition(const mat4& matrix)
    {
        return vec3(matrix.e[3], matrix.e[7], matrix.e[11]);
    }

	template<typename T>
	vec3& operator*=(vec3& lhs, const T& scale)
	{
		lhs = lhs * scale;

		return lhs;
	}

	template<typename T>
    vec3 operator/(const vec3& lhs, const T& scale)
	{
		if (scale == 0.f)
		    return { 0.f, 0.f, 0.f };

		T quotient = 1.f / scale;

		return { lhs.x * quotient, lhs.y * quotient, lhs.z * quotient };
	}

	template<typename T>
	vec3& operator/=(const vec3& lhs, const T& scale)
	{
		if (scale != 0.f)
		    return lhs;

		lhs = lhs / scale;

		return lhs;
	}

    template <typename T>
    T lerp(T v0, T v1, float t) { return (1 - t) * v0 + t * v1; }

	inline float dot(const vec3& vecA, const vec3& vecB)
    {
		return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
	}

    inline vec3 operator+(const vec3& lhs, const vec3& rhs)
	{
		return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	}

    inline vec3 operator-(const vec3& cur)
	{
		return { -cur.x, -cur.y, -cur.z };
	}

    inline vec3 operator-(const vec3& lhs, const vec3& rhs)
	{
		return lhs + -rhs;
	}
        
    inline void operator+=(vec3& lhs, const vec3& rhs)
    {
        lhs = lhs + rhs;
    }
        
    inline void operator-=(vec3& lhs, const vec3& rhs)
    {
        lhs = lhs - rhs;
    }

	// Cross product
    inline vec3 operator^(const vec3& lhs, const vec3& rhs)
	{
		return
		{
			lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x
		};
	}
        
	template<typename T>
	vec3 operator*(const vec3& vec, const T& scale)
	{
		return { vec.x * scale, vec.y * scale, vec.z * scale };
	}

	template<typename T>
	vec3 operator*(const T& scale, const vec3& vec)
	{
		return vec * scale;
    }

    inline vec3 reflect(const vec3& toReflect, const vec3& normal);

    inline quat operator*(const quat& lhs, const quat& rhs)
    {
        return {
            lhs.x * rhs.w + lhs.w * rhs.x + lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.y * rhs.w + lhs.w * rhs.y + lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.z * rhs.w + lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x,
            lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
        };
    }

    inline quat& operator*=(quat& lhs, const quat& rhs)
    {
        lhs = lhs * rhs;
        return lhs;
    }
}

#include "maths.inl"