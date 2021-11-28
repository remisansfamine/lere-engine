#version 450 core

layout (location = 0) in vec3 VertPos;
layout (location = 1) in vec3 VertTexCoords;
layout (location = 2) in vec3 VertTangent;
layout (location = 3) in vec3 VertBitangent;
layout (location = 4) in vec3 VertNormal;

//#define USE_NORMAL_MAP

out VS_OUT
{
	vec3 FragPos;
	vec3 TangentFragPos;
	vec3 TexCoord;
	vec3 TangentViewPos;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
	mat3 TBN;
} vs_out;


uniform vec2 tilling;
uniform vec3 viewPos;
uniform mat4 viewProj;
uniform mat4 model;

void main()
{
	vec4 fragPos = model * vec4(VertPos, 1.0);
	gl_Position = viewProj * fragPos;
	vs_out.FragPos = fragPos.xyz;
	
	vs_out.TexCoord = VertTexCoords * tilling.x + tilling.y;

	vec3 Normal = normalize(vec3(model * vec4(VertNormal, 0.0)));
	vs_out.Normal = Normal;

	#ifdef USE_NORMAL_MAP
		vec3 Tangent = normalize(vec3(model * vec4(VertTangent, 0.0)));
		vec3 Bitangent = normalize(vec3(model * vec4(VertBitangent, 0.0)));

		Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);

		float sign = dot(cross(Normal, Tangent), Bitangent) < 0.0 ? -1.0 : 1.0;
		Bitangent = sign * cross(Normal, Tangent);

		vs_out.Tangent = Tangent;
		vs_out.Bitangent = Bitangent;

		vs_out.TBN = mat3(Tangent, Bitangent, Normal);
	#else
		vs_out.TBN = mat3(1.0);
	#endif

	vs_out.TangentFragPos = vs_out.TBN * vs_out.FragPos;
	vs_out.TangentViewPos = vs_out.TBN * viewPos;
}