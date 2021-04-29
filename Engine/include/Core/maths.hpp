#pragma once

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
            float e[9];
            vec3  c[3];
        };

        union mat4
        {
            mat4() = default;
            float e[16];
            vec4  c[4];
        };

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
