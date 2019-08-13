#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 3) in vec2 vertexTexCoord;

// Values that stay constant for the whole mesh.
uniform mat4 model;
uniform mat4 lightVP;

out vec2 texCoord;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  lightVP * model * vec4(vertexPosition_modelspace, 1);

	//pass through
	texCoord = vertexTexCoord;
}

