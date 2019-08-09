#version 330 core

//just render a texture

// Interpolated values from the vertex shaders
in vec4 fragColor;
in vec2 texCoord;

// Ouput data
out vec4 color;

// misc uniforms
struct Material
{
	sampler2D colorMap[1];
};
uniform Material material;

void main()
{
	color = fragColor * texture2D(material.colorMap[0], texCoord);
}