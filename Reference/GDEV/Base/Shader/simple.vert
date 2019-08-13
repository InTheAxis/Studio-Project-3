#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 model, view, proj;

void main()
{
	gl_Position = proj * view * model * vec4(vertexPosition_modelspace, 1);
}
