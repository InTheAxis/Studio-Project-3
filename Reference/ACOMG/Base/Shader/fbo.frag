#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D screenTexture;

void main() 
{
	fragColor = texture2D(screenTexture, texCoord);
}

