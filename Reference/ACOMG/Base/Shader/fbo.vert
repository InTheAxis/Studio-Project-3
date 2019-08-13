#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 3) in vec2 vertexTexCoord;

uniform mat4 model;

out vec2 texCoord;

void main()
{	
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  model * vec4(vec2(vertexPosition_modelspace), 0, 1);

	//pass through
	texCoord = vertexTexCoord;
}

