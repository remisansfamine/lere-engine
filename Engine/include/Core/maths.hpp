#pragma once

#include <cmath>

namespace Core
{
    namespace Maths
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

        inline vec3 operator-(const vec3& vec)
        {
            return { -vec.x, -vec.y, -vec.z };
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

        // Functions

        // Clamp float
        /*inline float clamp(float x, float min, float max)
        {
            if (x > max) return max;
            if (x < min) return min;
            return x;
        }

        // Clamp vec3
        inline vec3 clamp(vec3 vec, float min, float max)
        {
            vec3 vecClamp;

            vecClamp.x = clamp(vec.x, min, max);
            vecClamp.y = clamp(vec.y, min, max);
            vecClamp.z = clamp(vec.z, min, max);

            return vecClamp;
        }

        inline float map(float x, float in_min, float in_max, float out_min, float out_max)
        {
            return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }*/

        template <typename T>
        inline T lerp(T v0, T v1, float t) { return (1 - t) * v0 + t * v1; }
    };
}
