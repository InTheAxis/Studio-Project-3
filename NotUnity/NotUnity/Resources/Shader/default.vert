#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aVertPos;
layout(location = 1) in vec4 aVertColor;
layout(location = 2) in vec3 aVertNormal;
layout(location = 3) in vec2 aVertTexCoord;

// Output data ; will be interpolated for each fragment.
out vec4 fragColor;
out vec2 texCoord;
out vec3 vertexPos_cameraspace;
out vec3 vertNormal_cameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 model, view, proj;

void main()
{
	gl_Position = proj * view * model * vec4(aVertPos, 1);	
	vertexPos_cameraspace = (view * model * vec4(aVertPos, 1)).xyz;		
	vertNormal_cameraspace = (transpose(inverse(view * model)) * vec4(aVertNormal, 0)).xyz;
	
	//pass throughs
	fragColor = aVertColor;
	texCoord = aVertTexCoord;
}