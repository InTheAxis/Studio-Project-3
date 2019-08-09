#version 330 core

//just render on screen

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aVertPos;
layout(location = 1) in vec4 aVertColor;
layout(location = 2) in vec3 aVertNormal;
layout(location = 3) in vec2 aVertTexCoord;

// Output data ; will be interpolated for each fragment.
out vec4 fragColor;
out vec2 texCoord;

// Values that stay constant for the whole mesh.
uniform mat4 model;

void main(){
	model * vec4(aVertPos, 1);

	//pass throughs
	fragColor = aVertColor;
	texCoord = aVertTexCoord;
}