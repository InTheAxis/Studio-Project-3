#version 330 core

//just render a texture

// Interpolated values from the vertex shaders
in vec4 fragColor;
in vec2 texCoord;

// Ouput data
out vec4 color;

uniform sampler2D colorTexture;

void main()
{
	color = fragColor * texture2D(colorTexture, texCoord);
}