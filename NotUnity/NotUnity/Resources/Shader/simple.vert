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
out vec3 vertPos_modelSpace;

// Values that stay constant for the whole mesh.
uniform mat4 model;

void main()
{
	//pass throughs
	fragColor = aVertColor;
	texCoord = aVertTexCoord;

	vec4 temp = model * vec4(aVertPos, 1);

	vertPos_modelSpace = vec3(temp);
	
	gl_Position = vec4(temp.xy, -1, 1);	
}