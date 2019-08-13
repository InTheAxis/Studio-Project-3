#version 330 core

//deafult but with vert offset

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

// Values that stay constant for the whole mesh.
uniform mat4 model, view, proj;
uniform float vertOffset;

void main(){
	// Output position of the vertex, in clip space : MVP * position
	vec4 vertPos = vec4(vertexPosition_modelspace, 1);
	vertPos.y = sin(vertPos.x + vertOffset) + cos(vertPos.z + vertOffset);
	vertPos = proj * view * model * vertPos;
	gl_Position  = vertPos;
	// Vector position, in camera space
	vertexPosition_cameraspace = (view * model * vec4(vertexPosition_modelspace, 1)).xyz;	
	// Vertex normal, in camera space
	vertexNormal_cameraspace = (transpose(inverse(view * model)) * vec4(vertexNormal_modelspace, 0)).xyz;
	
	//pass throughs
	fragmentColor = vertexColor;
	texCoord = vertexTexCoord;
}

