#version 450 core

in vec3 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

out vec4 FragColor;

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
	float shininess;

	sampler2D alphaTexture;
	sampler2D ambientTexture;
	sampler2D diffuseTexture;
	sampler2D emissiveTexture;
	sampler2D specularTexture;
};

uniform Material material;

struct Light
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	vec3 attenuation;
	float spotCutoff;

	vec3 spotDirection;
	float spotOuterCutoff;

	bool enable;
};

#define NBR_LIGHTS 8
Light lights[NBR_LIGHTS];

uniform mat4 lightAttribs2[NBR_LIGHTS][1];
uniform mat4 lightAttribs1[NBR_LIGHTS][1];

void parseLights()
{
	// Parse each light matrix to a light struct
	for (int i = 0; i < NBR_LIGHTS; i++)
	{
		lights[i].position	= lightAttribs1[i][0][0];
		lights[i].ambient	= lightAttribs1[i][0][1];
		lights[i].diffuse	= lightAttribs1[i][0][2];
		lights[i].specular	= lightAttribs1[i][0][3];

		lights[i].attenuation		= vec3(lightAttribs2[i][0][0]);
		lights[i].spotCutoff		= cos(radians(lightAttribs2[i][0][0][3]));
		lights[i].spotDirection		= vec3(lightAttribs2[i][0][1]);
		lights[i].spotOuterCutoff	= cos(radians(lightAttribs2[i][0][1][3]));
		lights[i].enable			= bool(lightAttribs2[i][0][2][0]);
	}
}

void getLightColor(out vec4 ambient, out vec4 diffuse, out vec4 specular)
{
	ambient = diffuse = specular = vec4(0.f, 0.f, 0.f, 1.f);

	// Initialize some variables to avoid to calculate them another time
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	for (int i = 0; i < NBR_LIGHTS; i++)
	{
		if (!lights[i].enable)
			continue;

		// Get light direction, if the light is a point light or a directionnal light
		vec3 lightDir = lights[i].position.xyz - FragPos * lights[i].position.w;
	
		// Compute the light direction and the distance between the fragment and the light
		float distance = length(lightDir);

		// Normalize the light direction manually
		lightDir /= distance;

		float finalIntensity = 1.f;

		// If the light is not a directionnal light, compute the final intensity
		if (lights[i].position.w != 0.f)
		{
			// Get spot cutoff and spot intensity 
			float theta = dot(lightDir, normalize(lights[i].spotDirection));
			float spotIntensity = clamp((theta - lights[i].spotOuterCutoff) / (lights[i].spotCutoff - lights[i].spotOuterCutoff), 0.f, 1.f);

			// Get attenuation (c + l * d + q * d²)
			float attenuation = lights[i].attenuation.x +
								lights[i].attenuation.y * distance +
								lights[i].attenuation.z * distance * distance;

			// Apply attenuation  to the spot intensity
			finalIntensity = spotIntensity / attenuation;
		}

		// Pre-compute normal ElightDir
		float NdotL = dot(normal, lightDir);

		// Compute ambient
		ambient += lights[i].ambient;

		// Compute diffuse
		diffuse += max(NdotL, 0.f) * finalIntensity * lights[i].diffuse;

		// Compute specular
		vec3 reflectDir = 2.f * NdotL * normal - lightDir;  
		specular += pow(max(dot(viewDir, reflectDir), 0.f), material.shininess) * finalIntensity * lights[i].specular;
	}
}

void main()
{
	parseLights();

	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 diffuse = vec4(1.0);
	vec4 specular = vec4(0.0);

	getLightColor(ambient, diffuse, specular);

	vec4 ambientColor = ambient * (material.ambient + texture(material.ambientTexture, TexCoord.st));

	vec4 diffuseColor = material.diffuse * diffuse;

	vec4 specularColor = specular * (material.specular + texture(material.specularTexture, TexCoord.st));

	vec4 emissiveColor = material.emissive + texture(material.emissiveTexture, TexCoord.st);

	// Get texture color applied to the light
	vec4 shadedColor = (ambientColor + diffuseColor) * texture(material.diffuseTexture, TexCoord.st) + emissiveColor + specularColor;
	FragColor = shadedColor;

	FragColor.a = texture(material.alphaTexture, TexCoord.st).r;
}