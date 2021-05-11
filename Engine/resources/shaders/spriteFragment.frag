#version 450 core

in vec3 TexCoords;

uniform sampler2D diffuseTex;
uniform vec4 color;

out vec4 FragColor;

void main()
{
	FragColor = color * texture(diffuseTex, TexCoords.st);
}