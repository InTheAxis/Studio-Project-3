#version 330 core

//deafult but with billboarding

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

void main(){
	// Output position of the vertex, in clip space : MVP * position	
	vec3 cameraRight_worldspace = vec3(view[0][0], view[1][0], view[2][0]);
	vec3 cameraUp_worldspace = vec3(view[0][1], view[1][1], view[2][1]);
	vec3 vertTranslate = vec3(model[3][0], model[3][1], model[3][3]);
	vec2 vertScale = vec2(model[0][0], model[1][1]);

	vec3 vertPos = vertTranslate + vertexPosition_modelspace.x * cameraRight_worldspace * vertScale.x + vertexPosition_modelspace.y * cameraUp_worldspace * vertScale.y;
	
	gl_Position = proj * view * vec4(vertPos, 1);
	// Vector position, in camera space
	vertexPosition_cameraspace = (view * model * vec4(vertexPosition_modelspace, 1)).xyz;	
	// Vertex normal, in camera space
	vertexNormal_cameraspace = (transpose(inverse(view * model)) * vec4(vertexNormal_modelspace, 0)).xyz;
	
	//pass throughs
	fragmentColor = vertexColor;
	texCoord = vertexTexCoord;
}

