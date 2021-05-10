#version 450 core

in vec3 TexCoords;

uniform sampler2D diffuseTex;

out vec4 FragColor;

void main()
{
	FragColor = texture(diffuseTex, TexCoords.st);
}