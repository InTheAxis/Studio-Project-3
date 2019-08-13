#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec2 vertexTexCoord;

// Output data ; will be interpolated for each fragment.
out vec3 vertexPosition_cameraspace;
out vec4 fragmentColor;
out vec3 vertexNormal_cameraspace;
out vec2 texCoord;
out vec3 position_modelspace;
out vec4 shadowCoord;
// Values that stay constant for the whole mesh.
uniform mat4 model, view, proj;
uniform mat4 lightDepthMVP;

void main(){
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  proj * view * model * vec4(vertexPosition_modelspace, 1);
	
	// Vector position, in camera space
	vertexPosition_cameraspace = (view * model * vec4(vertexPosition_modelspace, 1)).xyz;	
	// Vertex normal, in camera space
	vertexNormal_cameraspace = (transpose(inverse(view * model)) * vec4(vertexNormal_modelspace, 0)).xyz;
	
	// Output position of the vertex, in light space : lightDepthMVP * position
	shadowCoord = lightDepthMVP * vec4(vertexPosition_modelspace, 1);
	shadowCoord = shadowCoord / shadowCoord.w;
	shadowCoord.xyz = shadowCoord.xyz * 0.5 + 0.5;

	//pass throughs
	fragmentColor = vertexColor;
	texCoord = vertexTexCoord;
	position_modelspace =  (model * vec4(vertexPosition_modelspace, 1)).xyz;
}

