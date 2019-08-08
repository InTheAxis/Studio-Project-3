#version 330 core

//override fragment color

// Interpolated values from the vertex shaders
in vec3 vertexPosition_cameraspace;
in vec4 fragmentColor;
in vec3 vertexNormal_cameraspace;
in vec2 texCoord;

// Ouput data
out vec4 color;

//misc uniforms
uniform vec4 outColor;

void main()
{
	color = vec4(1);
}

