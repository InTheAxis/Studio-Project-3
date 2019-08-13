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
	vec4 cameraRight_worldspace = vec4(view[0][0], view[1][0], view[2][0], 0);
	vec4 cameraUp_worldspace = vec4(view[0][1], view[1][1], view[2][1], 0);
	vec3 vertTranslate = vec3(model[3][0], model[3][1], model[3][2]);
	vec2 vertScale = vec2(model[0][0], model[1][1]);

	vec3 vertPos = vertTranslate + vertexPosition_modelspace.x * vec3(cameraRight_worldspace) * vertScale.x + vertexPosition_modelspace.y * vec3(cameraUp_worldspace) * vertScale.y;

//	mat4 newModelView = view * model;
//	newModelView[0][0] = vertScale.x;
//	newModelView[0][1] = 0;
//	newModelView[0][2] = 0;
//	newModelView[1][0] = 0;
//	newModelView[1][1] = vertScale.y;
//	newModelView[1][2] = 0;
//	newModelView[2][0] = 0;
//	newModelView[2][1] = 0;
//	newModelView[2][2] = 1;

	gl_Position = proj * view * vec4(vertPos, 1);
	// Vector position, in camera space
	vertexPosition_cameraspace = (view * model * vec4(vertexPosition_modelspace, 1)).xyz;	
	// Vertex normal, in camera space
	vertexNormal_cameraspace = (transpose(inverse(view * model)) * vec4(vertexNormal_modelspace, 0)).xyz;
	
	//pass throughs
	fragmentColor = vertexColor;
	texCoord = vertexTexCoord;
}

